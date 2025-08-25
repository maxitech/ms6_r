#include "ProgramLoader.h"
#include "DebugLog.h"
#include "Setup.h"
#include "Utils.h"
#include <map>

#define LOG(level, msg) DebugLog::log(level, msg)

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
    if (processedDta.cmdId == NOP) // replace with other cmd
    {
        Utils::createAndSendPacket(processedDta.cmdId, STATUS_OK, WARN_NOP_IGNORED);
        LOG(LOG_WARN, "No operation command! No execution.");
        return;
    }

    _processedDta            = processedDta;
    _cmdId                   = processedDta.cmdId;
    const bool isReqTelemety = processedDta.is_requestTelemetry.value();

    // Optional payload data
    std::optional<uint8_t> program;
    if (processedDta.program.has_value())
    {
        program = processedDta.program.value();
    }

    if (processedDta.jogSpeeds.has_value())
    {
        _jogSpeeds = processedDta.jogSpeeds.value();
    }

    if (_cmdId == CMD_LOAD && program)
    {
        _loadProgram(program.value());
    }
    else if (_cmdId == CMD_START)
    {
        if (_currentProgramState == MAIN)
        {
            return;
        }
        _start();
    }
    else if (_cmdId == CMD_STOP)
    {
        if (_currentProgramState == MAIN)
        {
            return;
        }
        digitalWrite(_limitSwitches.getLedPin(), LOW); // Turn off the LED
        _stop();
    }
    else if (_cmdId == CMD_IDLE)
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
    else if (_cmdId == CMD_JOG || _cmdId == CMD_MOVE_TO_POS)
    {
        if (_currentProgramState != MAIN)
        {
            _loadProgram(PRG_MAIN);
        }
    }
}

void ProgramLoader::_loadProgram(const uint8_t program)
{
    _stop(); // Stop any running program before loading a new one
    static const std::map<String, ProgramState> programMap = {
        {PRG_PING, PING},
        {PRG_PONG, PONG},
        {PRG_TEST_SWITCHES, TEST_SWITCHES},
        {PRG_HOME, HOME},
        {PRG_MAIN, MAIN},
    };

    auto it = programMap.find(program);
    if (it == programMap.end())
    {
        Utils::createAndSendPacket(_cmdId, STATUS_ERROR, ERR_UNKNOWN_PROGRAM);
        LOG(LOG_ERROR, "Unknown program.");

        return;
    }

    if (_currentProgramState == it->second)
    {
        Utils::createAndSendPacket(_cmdId, STATUS_OK, INFO_RELOADED_PROGRAM);
        LOG(LOG_INFO, "Reloaded program.");
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

        Utils::createAndSendPacket(_cmdId, STATUS_OK, INFO_LOADED_PROGRAM);
        LOG(LOG_INFO, "Loaded program.");
    }
}

void ProgramLoader::_start()
{
    if (_currentProgramState == IDLE)
    {
        Utils::createAndSendPacket(_cmdId, STATUS_OK, WARN_NO_PROGRAM_LOADED);
        LOG(LOG_WARN, "No program loaded.");
        return;
    }

    if (_executionState == EXEC_RUNNING)
    {
        Utils::createAndSendPacket(_cmdId, STATUS_OK, INFO_ALREADY_RUNNING);
        LOG(LOG_INFO, "Program already running.");
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
    _eStopMotors(); // Stop all motors if they are moving
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
        Utils::createAndSendPacket(NOP, STATUS_OK, DATA_NONE);
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
            _isHomingDone = true; // Set homing done flag

            // !!! Maybe reset homing state later
            uint8_t homedMask = _homingManager->getHomedMask();
            _rbtDtaSender.setHomedMask(homedMask);
            _rbtDtaSender.sendMotorPosInSteps(_motorConfigs); // ! set cmdId

            _executionState = EXEC_IDLE;
            _setState(IDLE);
            return;
        }
    }
    else
    {
        _executionState = EXEC_IDLE;
        _setState(IDLE);

        Utils::createAndSendPacket(_cmdId, STATUS_OK, INFO_HOMING_ALREADY_DONE);
        LOG(LOG_INFO, "Homing already done, skipping.");
    }
}

void ProgramLoader::_main()
{
    static bool     warningShown = false;
    static JogState currJogState = IDLE_JOG;

    if (_cmdId == CMD_JOG && _isHomingDone)
    {
        _jogCtrl->jogJoint(_jogSpeeds, currJogState);
        if (warningShown)
        {
            warningShown = false;
        }
    }
    else
    {
        currJogState = IDLE_JOG;
        if (!warningShown)
        {
            Utils::createAndSendPacket(_cmdId, STATUS_OK, WARN_ARM_NOT_HOMED);
            LOG(LOG_WARN, "Arm not homed - home first");
            warningShown = true;
        }
    }
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