#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(ProgramLoader& programLoader)
    : _programLoader(programLoader) {};

void CommandDispatcher::dispatch(const std::vector<String>& cmdParts)
{
    if (cmdParts.empty())
        return;

    const String& command = cmdParts[0];

    // --------------------This block is use if backend runs main func automatically if JOG of MOVE are the cmd's-------------
    if (command == "LOAD" || command == "JOG" || command == "MOVE")
    {
        _programLoader.handleCommand(cmdParts);
    }
    else
    {
        Serial.println("Unknown command: " + command);
        delay(20);
    }

    // -------------------This block can be used if main func running has to be handled by the frontend------------------------
    // if (command == "LOAD")
    // {
    //     _programLoader.handleCommand(cmdParts);
    // }
    // else if (command == "JOG" || command == "MOVE")
    //     if (_programLoader.getState() == MAIN)
    //     {
    //         _programLoader.handleCommand(cmdParts);
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
