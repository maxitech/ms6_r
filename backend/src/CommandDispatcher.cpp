#include "CommandDispatcher.h"
#include "Setup.h"

CommandDispatcher::CommandDispatcher(ProgramLoader& programLoader)
    : _programLoader(programLoader) {};

void CommandDispatcher::dispatch(const String& cmd, const std::vector<String>& args)
{
    if (cmd == "" || args.empty())
    {
        Serial.println("Error: no command or no arguments");
        return;
    }

    const String& command = cmd;

    // --------------------This block is use if backend runs main func automatically if JOG of MOVE are the cmd's-------------
    std::vector<String> cmds       = {"LOAD", "START", "STOP", "IDLE", "HOME", "JOG", "MOVE", "SETUP"};
    bool                isValidCmd = std::find(cmds.begin(), cmds.end(), command) != cmds.end();
    if (isValidCmd)
    {
        if (cmd == "SETUP")
        {
            Setup setupInstance(args[0]);
        }
        else
        {
            _programLoader.handleCommand(cmd, args);
        }
    }
    else
    {
        Serial.println("Error: Unknown command or command invalide! Correct format <$cmd,[arg, arg, ...]>" + command);
    }

    // -------------------This block can be used if main func running has to be handled by the frontend------------------------
    // if (command == "LOAD")
    // {
    //     _programLoader.handleCommand(args);
    // }
    // else if (command == "JOG" || command == "MOVE")
    //     if (_programLoader.getState() == MAIN)
    //     {
    //         _programLoader.handleCommand(args);
    //     }
    //     else
    //     {
    //         Serial.println("Error: Main program must be loaded first!");
    //         delay(20);
    //     }
    // else
    // {
    //     Serial.println("Unknown command: " + command);
    //     delay(20);
    // }
}
