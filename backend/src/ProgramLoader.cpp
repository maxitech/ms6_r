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
        delay(20);
        return;
    }

    const String& command = cmd;
    const String& program = args[0];
    _arguments            = args;

    for (String& arg : _arguments)
        arg.trim();

    if (command == "LOAD")
    {
        _loadProgram(program);
    }
    else if (command == "START")
    {
        if (_currentProgramState != MAIN)
        {
            _start();
        }
        else
        {
            return;
        }
    }
    else if (command == "STOP")
    {
        if (_currentProgramState != MAIN)
        {
            _stop();
        }
        else
        {
            return;
        }
    }
    else if (command == "JOG" || command == "MOVE")
    {

        if (_currentProgramState != MAIN)
        {
            _loadProgram("MAIN");
            _cmd = command;
        }
        else
        {
            _cmd = command;
        }
    }
}

void ProgramLoader::_loadProgram(const String& program)
{
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
        delay(20);
        return;
    }

    if (_currentProgramState == it->second)
    {
        Serial.println("Reloaded program: " + program);
        delay(20);
    }
    else
    {
        Serial.println("Loading program: " + program);
        delay(20);
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
        delay(20);
    }
}

void ProgramLoader::_start()
{
    if (_currentProgramState == IDLE)
    {
        Serial.println("No program loaded.");
        delay(20);
        return;
    }

    if (_executionState == EXEC_RUNNING)
    {
        Serial.println("Program already running.");
        delay(20);
        return;
    }

    Serial.print("Starting program");
    delay(20);
    _executionState = EXEC_RUNNING;
}

void ProgramLoader::_stop()
{
    if (_executionState != EXEC_RUNNING)
    {
        Serial.println("No program is running.");
        delay(20);
        return;
    }

    Serial.println("Stopping program");
    delay(20);
    // Perform any necessary cleanup here
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
        Serial.println("[INFO] - Homing done going back to IDLE state.");
        delay(20);
        _setState(IDLE);
    }
}

void ProgramLoader::_main()
{
    if (_arguments.empty())
    {
        Serial.println("Warning: _arguments vector is empty!");
        delay(20);
        return;
    }
    const String& joint     = _arguments[0];
    String&       direction = _arguments[1];
    int           motorIdx  = joint.substring(1).toInt() - 1;
    const int     velocity  = 1000;
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
                    delay(20);
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
            delay(20);
            break;
        }
    }
    if (currJogState == IDLE_JOG)
    {
        delay(20);
    }
}

JogCommand ProgramLoader::_getJogCommand(const String& str)
{
    if (str == "START")
        return JOG_START;
    else if (str == "STOP")
        return JOG_STOP;
    else
        return JOG_UNKNOWN;
}