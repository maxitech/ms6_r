#include "ProgramLoader.h"
#include "Utils.h"
#include <map>

ProgramLoader::ProgramLoader(Homing* homingManager, std::vector<MotorConfig>& configs, LimitSwitches& limitSwitches)
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
            _stop(); // Stop any running program and set to IDLE state
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
    if (!_homingManager->isHomingDone())
    {
        _homingManager->executeHoming();
    }
    else
    {
        // Maybe reset homing state later
        for (size_t i = 0; i < _motorConfigs.size(); ++i)
        {
            const int motorPos = _motorConfigs[i].motor->getPosition();
            Serial.print("DATA:MOTOR_POS_STEPS*");
            Serial.print(static_cast<int>(i + 1));
            Serial.print("#");
            Serial.println(motorPos);
        }
        _executionState = EXEC_IDLE;
        _setState(IDLE);
    }
}

void ProgramLoader::_main()
{
    if (_arguments.empty())
    {
        Serial.println("Warning: _arguments vector is empty!");
        return;
    }
    const String& joint     = _arguments[0];
    String&       direction = _arguments[1];
    int           motorIdx  = joint.substring(1).toInt() - 1;
    const int     velocity  = _arguments[2].toInt();
    const String& jogState  = _arguments.back();

    static JogState currJogState = IDLE_JOG;

    if (_cmd == "JOG")
    {
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

                _motorConfigs[motorIdx].motor->rotateAsync(dirVel);
            }
            break;

        case JOG_STOP:
            if (currJogState != IDLE_JOG)
            {
                // stop
                _motorConfigs[motorIdx].motor->emergencyStop();
                currJogState = IDLE_JOG;
            }
            break;

        case JOG_UNKNOWN:
        default:
            Serial.println("Unknown JOG state: " + jogState);
            break;
        }
    }

    if (currJogState == JOGGING && _homingManager->isHomingDone())
    {
        static unsigned long lastSendTime = 0;
        if (Utils::nonBlockingDelay(100, lastSendTime))
        {
            const int motorPos = _motorConfigs[motorIdx].motor->getPosition();
            Serial.print("DATA:MOTOR_POS_STEPS*");
            Serial.print(motorIdx + 1);
            Serial.print("#");
            Serial.println(motorPos);
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

void ProgramLoader::_stopMotors()
{
    for (MotorConfig& cfg : _motorConfigs)
    {
        if (cfg.motor != nullptr && cfg.motor->isMoving)
        {
            cfg.motor->emergencyStop(); // Stop all motors if they are moving
        }
    }
}