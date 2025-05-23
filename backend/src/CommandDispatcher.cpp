#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(ProgramLoader& programLoader)
    : _programLoader(programLoader) {};

void CommandDispatcher::dispatch(const String& cmd, const std::vector<String>& args)
{
    if (cmd == "" || args.empty())
    {
        Serial.println("Error: no command or no arguments");
        delay(20);
        return;
    }

    const String& command = cmd;

    // --------------------This block is use if backend runs main func automatically if JOG of MOVE are the cmd's-------------
    if (command == "LOAD" || command == "JOG" || command == "MOVE")
    {
        _programLoader.handleCommand(cmd, args);
    }
    else
    {
        Serial.println("Unknown command: " + command);
        delay(20);
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
