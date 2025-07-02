#include "ProgramLoader.h"
#include "Setup.h"
#include "Utils.h"
#include <map>

ProgramLoader::ProgramLoader(Homing* homingManager, std::vector<MotorConfig*>& configs, LimitSwitches& limitSwitches)
    : _homingManager(homingManager)
    , _motorConfigs(configs)
    , _limitSwitches(limitSwitches) {};

void ProgramLoader::handleCommand(const String& cmd, const std::vector<String>& args)
{
    if (cmd == "" || args.empty())
    {
        Serial.println("Error: False command or not enough arguments.");
        return;
    }

    const String& command = cmd;
    const String& program = args[0];
    _arguments            = args;
    _cmd                  = command;

    for (String& arg : _arguments)
        arg.trim();

    // Note: Because of command being a String I cannot use switch-case here. -> could use mapping but not needed atm.
    if (command == "LOAD")
    {
        _loadProgram(program);
    }
    else if (command == "START")
    {
        if (_currentProgramState == MAIN)
        {
            return;
        }
        _start();
    }
    else if (command == "STOP")
    {
        if (_currentProgramState == MAIN)
        {
            return;
        }
        digitalWrite(_limitSwitches.getLedPin(), LOW); // Turn off the LED
        _stop();
    }
    else if (command == "IDLE")
    {
        if (_currentProgramState == IDLE)
        {
            return;
        }
        else
        {
            _stop();                                       // Stop any running program and set to IDLE state
            digitalWrite(_limitSwitches.getLedPin(), LOW); // Turn off the LED
            _setState(IDLE);
            _executionState = EXEC_IDLE;
        }
    }
    else if (command == "JOG" || command == "MOVE")
    {

        if (_currentProgramState != MAIN)
        {
            _loadProgram("MAIN");
        }
    }
}

void ProgramLoader::_loadProgram(const String& program)
{
    _stop(); // Stop any running program before loading a new one
    static const std::map<String, ProgramState> programMap = {
        {"PING", PING},
        {"PONG", PONG},
        {"TEST_SWITCHES", TEST_SWITCHES},
        {"HOME", HOME},
        {"MAIN", MAIN},
    };

    auto it = programMap.find(program);
    if (it == programMap.end())
    {
        Serial.println("Unknown program: " + program);
        return;
    }

    if (_currentProgramState == it->second)
    {
        Serial.println("Reloaded program: " + program);
    }
    else
    {
        _setState(it->second);
        if (it->second == MAIN)
        {
            _executionState = EXEC_RUNNING; // Automatically start MAIN program
        }
        else
        {
            _executionState = EXEC_IDLE; // Set to idle for other programs
        }

        Serial.println("Loaded program: " + program);
    }
}

void ProgramLoader::_start()
{
    if (_currentProgramState == IDLE)
    {
        Serial.println("No program loaded.");
        return;
    }

    if (_executionState == EXEC_RUNNING)
    {
        Serial.println("Program already running.");
        return;
    }
    _executionState = EXEC_RUNNING;
}

void ProgramLoader::_stop()
{
    if (_executionState != EXEC_RUNNING)
    {
        return;
    }

    // Clean up
    _stopMotors(); // Stop all motors if they are moving
    _executionState = EXEC_IDLE;
}

void ProgramLoader::run()
{
    if (_currentProgramState == IDLE || _executionState != EXEC_RUNNING)
    {
        return;
    }

    switch (_currentProgramState)
    {
    case PING:
        _executePing();
        break;
    case PONG:
        _executePong();
        break;
    case TEST_SWITCHES:
        _testSwitches();
        break;
    case HOME:
        _home();
        break;
    case MAIN:
        _main();
        break;
    case IDLE:
    default:
        break;
    }
}

ProgramState ProgramLoader::_setState(ProgramState newState)
{
    _currentProgramState = newState;
    return _currentProgramState;
}

ProgramState ProgramLoader::getState() const
{
    return _currentProgramState;
}

//  ******************************PROGRAMS********************************
void ProgramLoader::_executePing()
{
    static unsigned long lastPingTime = 0;
    if (Utils::nonBlockingDelay(1000, lastPingTime))
    {
        Serial.println("PONG");
    }
}

void ProgramLoader::_executePong()
{
    static unsigned long lastPongTime = 0;
    if (Utils::nonBlockingDelay(1000, lastPongTime))
    {
        Serial.println("PING");
    }
}

void ProgramLoader::_testSwitches()
{
    _limitSwitches.check();
}

void ProgramLoader::_home()
{
    if (!_isHomingDone)
    {

        _homingManager->executeHoming();
        if (_homingManager->isHomingDone())
        {
            _isHomingDone = true;        // Set homing done flag
            _sendFkPoseAndJointAngles(); // Send FK pose and joint angles after homing is done

            // Maybe reset homing state later
            for (size_t i = 0; i < _motorConfigs.size(); ++i)
            {
                _sendMotorPosInSteps(i); // Send motor position in steps
            }
            _executionState = EXEC_IDLE;
            _setState(IDLE);
            return;
        }
    }
    else
    {
        _executionState = EXEC_IDLE;
        _setState(IDLE);
        Serial.println("Homing already done, skipping.");
    }
}

void ProgramLoader::_main()
{
    if (_arguments.empty())
    {
        Serial.println("Warning: _arguments vector is empty!");
        return;
    }
    const String&   joint        = _arguments[0];                  // e.g. "J1"
    const int       motorIdx     = joint.substring(1).toInt() - 1; // Convert "J1" extract 1 -> to index 0
    static JogState currJogState = IDLE_JOG;

    if (_cmd == "JOG")
    {
        _jogJoint(currJogState, motorIdx);
    }

    if (currJogState == JOGGING && _homingManager->isHomingDone())
    {
        static unsigned long lastSendTime = 0;
        if (Utils::nonBlockingDelay(100, lastSendTime))
        {
            // Send motor position in steps
            _sendMotorPosInSteps(motorIdx);
            // Send forward kinematics pose and joint angles
            _sendFkPoseAndJointAngles();
        }
    }
    else if (currJogState == IDLE_JOG)
    {
        delay(20);
    }
}

//  ******************************HELPER FUNCTIONS********************************
JogCommand ProgramLoader::_getJogCommand(const String& str)
{
    if (str == "START")
        return JOG_START;
    else if (str == "STOP")
        return JOG_STOP;
    else
        return JOG_UNKNOWN;
}

void ProgramLoader::_jogJoint(JogState& currJogState, const int motorIdx)
{
    String&       direction = _arguments[1];
    const int     velocity  = _arguments[2].toInt();
    const String& jogState  = _arguments.back();

    JogCommand jogCmd = _getJogCommand(jogState);

    switch (jogCmd)
    {
    case JOG_START:
        if (currJogState != JOGGING)
        {
            // start
            currJogState = JOGGING;
            if (direction != "POS" && direction != "NEG")
            {
                Serial.println("Invalid direction string: " + direction);
                return;
            }

            int dir    = (direction == "POS") ? 1 : -1;
            int dirVel = velocity * dir;

            _motorConfigs[motorIdx]->motor->rotateAsync(dirVel);
        }
        break;

    case JOG_STOP:
        if (currJogState != IDLE_JOG)
        {
            // stop
            _motorConfigs[motorIdx]->motor->emergencyStop();
            currJogState = IDLE_JOG;
        }
        break;

    case JOG_UNKNOWN:
    default:
        Serial.println("Unknown JOG state: " + jogState);
        break;
    }
}

void ProgramLoader::_stopMotors()
{
    for (MotorConfig* cfg : _motorConfigs)
    {
        if (cfg->motor != nullptr && cfg->motor->isMoving)
        {
            cfg->motor->emergencyStop(); // Stop all motors if they are moving
        }
    }
}

void ProgramLoader::_sendFkPoseAndJointAngles()
{
    Pose                pose        = Setup::getInstance().getKinematics()->forwardKinematics();
    std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);

    // Send forward kinematics pose
    // Format: DATA:FK_POSE*x,y,z,roll,pitch,yaw
    Serial.print("DATA:FK_POSE*");
    Serial.print(pose.x);
    Serial.print(",");
    Serial.print(pose.y);
    Serial.print(",");
    Serial.print(pose.z);
    Serial.print(",");
    Serial.print(pose.roll);
    Serial.print(",");
    Serial.print(pose.pitch);
    Serial.print(",");
    Serial.println(pose.yaw);

    if (jointAngles.empty())
    {
        Serial.println("Error: jointAngles vector is empty!");
        return;
    }
    // Send joint angles
    // Format: DATA:JOINT_ANGLES*angle1,angle2,angle3
    Serial.print("DATA:JOINT_ANGLES*");
    for (size_t i = 0; i < jointAngles.size(); ++i)
    {
        Serial.print(jointAngles[i]);
        if (i < jointAngles.size() - 1)
        {
            Serial.print(",");
        }
    }
    Serial.println();
}

void ProgramLoader::_sendMotorPosInSteps(const int motorIdx)
{
    // Format: DATA:MOTOR_POS_STEPS*<motor_index>#<position>
    const int motorPos = _motorConfigs[motorIdx]->motor->getPosition();
    Serial.print("DATA:MOTOR_POS_STEPS*");
    Serial.print(motorIdx + 1);
    Serial.print(",");
    Serial.println(motorPos);
}