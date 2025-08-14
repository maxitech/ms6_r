#include "ProgramLoader.h"
#include "Setup.h"
#include "Utils.h"
#include <map>

using namespace CommunicationProtocoll;

ProgramLoader::ProgramLoader(Homing* homingManager, std::vector<MotorConfig*>& configs, LimitSwitches& limitSwitches)
    : _homingManager(homingManager)
    , _motorConfigs(configs)
    , _limitSwitches(limitSwitches)
    , _rbtDtaSender()
    , _jogCtrl(new JogController(_motorConfigs, _rbtDtaSender)) {};

ProgramLoader::~ProgramLoader()
{
    delete _jogCtrl;
}

void ProgramLoader::handleCommand(const ProcessedData& processedDta)
{
    if (processedDta.cmdId == NOP)
    {
        Serial.println("Warning: No operation command! No execution.");
        return;
    }

    _processedDta       = processedDta;
    const uint8_t cmdId = processedDta.cmdId;
    Serial.println(cmdId);
    const bool             isReqTelemety = processedDta.is_requestTelemetry.value();
    std::optional<uint8_t> program;
    if (processedDta.program.has_value())
    {
        program = processedDta.program.value();
    }

    std::optional<std::vector<int32_t>> jogSpeeds;
    if (processedDta.jogSpeeds.has_value())
    {
        jogSpeeds = processedDta.jogSpeeds.value();
    }

    if (cmdId == CMD_LOAD && program)
    {
        _loadProgram(program.value());
    }
    else if (cmdId == CMD_START)
    {
        if (_currentProgramState == MAIN)
        {
            return;
        }
        _start();
    }
    else if (cmdId == CMD_STOP)
    {
        if (_currentProgramState == MAIN)
        {
            return;
        }
        digitalWrite(_limitSwitches.getLedPin(), LOW); // Turn off the LED
        _stop();
    }
    else if (cmdId == NOP)
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
    else if (cmdId == CMD_JOG || cmdId == CMD_MOVE_TO_POS)
    {
        if (_currentProgramState != MAIN)
        {
            // _loadProgram("MAIN");
        }
    }
}

void ProgramLoader::_loadProgram(const uint8_t program)
{
    _stop(); // Stop any running program before loading a new one
    static const std::map<String, ProgramState> programMap = {
        {PRG_PING, PING},
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
            _executionState = EXEC_RUNNING; // Automatically start MAIN program
            // _executionState = EXEC_IDLE; // Set to idle for other programs
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
            _isHomingDone = true;                     // Set homing done flag
            _rbtDtaSender.sendFkPoseAndJointAngles(); // Send FK pose and joint angles after homing is done

            // Maybe reset homing state later
            for (size_t i = 0; i < _motorConfigs.size(); ++i)
            {
                _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[i], i); // Send motor position in steps
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
    // if (_arguments.empty())
    // {
    //     Serial.println("Warning: _arguments vector is empty!");
    //     return;
    // }
    // const String&   joint        = _arguments[0];                  // e.g. "J1"
    // const int       motorIdx     = joint.substring(1).toInt() - 1; // Convert "J1" extract 1 -> to index 0
    // static JogState currJogState = IDLE_JOG;
    // static bool     warningShown = false;

    // if (_cmd == CMD_JOG && _isHomingDone)
    // {
    //     _jogCtrl->jogJoint(_arguments, currJogState, motorIdx);
    //     warningShown = false;
    // }
    // else
    // {
    //     currJogState = IDLE_JOG;
    //     if (!warningShown)
    //     {
    //         Serial.println("Arm not homed - home first");
    //         warningShown = true;
    //     }
    // }
}

//  ******************************HELPER FUNCTIONS********************************

void ProgramLoader::_stopMotors()
{
    for (MotorConfig* cfg : _motorConfigs)
    {
        if (cfg->motor != nullptr && cfg->motor->isMoving)
        {
            cfg->motor->stopAsync(); // Stop all motors if they are moving(smooth)
        }
    }
}

void ProgramLoader::_eStopMotors()
{
    for (MotorConfig* cfg : _motorConfigs)
    {
        if (cfg->motor != nullptr && cfg->motor->isMoving)
        {
            cfg->motor->emergencyStop(); // Stop all motors if they are moving(hard)
        }
    }
}