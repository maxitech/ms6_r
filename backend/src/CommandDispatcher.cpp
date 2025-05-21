#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(ProgramLoader& programLoader)
    : _programLoader(programLoader) {};

void CommandDispatcher::dispatch(const std::vector<String>& cmdParts)
{
    if (cmdParts.empty())
        return;

    const String& command = cmdParts[0];

    if (command == "LOAD")
    {
        _programLoader.handleCommand(cmdParts);
    }
    else
    {
        Serial.println("Unknown command: " + command);
        delay(20);
    }
}
