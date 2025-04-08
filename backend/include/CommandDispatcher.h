/**
 * @file CommandDispatcher.h
 * @brief Declares the CommandDispatcher class that interprets parsed commands and delegates execution.
 */

#ifndef COMMANDDISPATCHER_H
#define COMMANDDISPATCHER_H

#include "ProgramLoader.h"
#include <Arduino.h>

/**
 * @brief Dispatches validated and parsed commands to the appropriate handler.
 *
 * This class currently supports the "LOAD" command, which forwards to the ProgramLoader instance.
 * Additional commands can be added here as needed.
 */
class CommandDispatcher
{
public:
    /**
     * @brief Constructs a CommandDispatcher with a reference to the ProgramLoader.
     * @param programLoader Reference to the ProgramLoader used to handle commands like LOAD.
     */
    explicit CommandDispatcher(ProgramLoader& programLoader);

    /**
     * @brief Dispatches the parsed command parts to the appropriate handler.
     * @param cmdParts Vector of strings where the first element is the command name.
     */
    void dispatch(const std::vector<String>& cmdParts);

private:
    /**
     * @brief Reference to the ProgramLoader used to handle specific commands.
     * @internal
     */
    ProgramLoader& _programLoader;
};

#endif // COMMANDDISPATCHER_H
