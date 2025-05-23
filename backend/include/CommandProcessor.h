/**
 * @file CommandProcessor.h
 * @brief Declares the CommandProcessor class responsible for validating and dispatching incoming serial commands.
 * @details This class is a core component of the robot control system, responsible for parsing serial input,
 *          verifying message integrity through checksums, and delegating commands for execution.
 */

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "CommandDispatcher.h"
#include "SerialHandler.h"
#include <Arduino.h>

/**
 * @class CommandProcessor
 * @brief Handles input commands and processes them, including validation and checksum verification.
 */
class CommandProcessor
{
public:
    /**
     * @brief Constructor for the CommandProcessor class.
     * @param programLoader Reference to a ProgramLoader instance used to initialize the dispatcher.
     * @note Marked as explicit to prevent unintended implicit conversions when passing a single argument.
     */
    explicit CommandProcessor(ProgramLoader& programLoader);

    /**
     * @brief Processes an input string, validates it, and if all valid, it forwards the data.
     * @param input The input string in the format $<cmd>*<checksum>#.
     */
    void processInput(const String& input);

private:
    /**
     * @brief Validates the input format.
     * @param input The input string to validate.
     * @return True if the input is valid, false otherwise.
     * @internal
     */
    bool _isInputValid(const String& input);

    /**
     * @brief Verifies the checksum of the command string.
     * @param data The portion of the string between `$` and `*`.
     * @param checksum The checksum value to validate against.
     * @return True if the checksum is correct, false otherwise.
     * @note The checksum is calculated using an XOR operation over all characters in `data`.
     * @internal
     */
    bool _validateChecksum(const String& data, const String& checksum);

    /**
     * @brief Processes a valid command.
     * @param cmd The data string (after validation).
     * @internal
     */
    void _processCommand(const String& cmd);

    /**
     * @brief Splits a string into parts based on a delimiter.
     * @param str The string to split.
     * @return A pair of String & vector of substrings.
     * @internal
     */
    // std::vector<String> _splitString(const String& str, const char delimiter);
    std::pair<String, std::vector<String>> _splitString(const String& str);

    /**
     * @brief Dispatcher used to handle commands internally.
     * @note Initialized with a reference to the ProgramLoader instance passed to the constructor.
     * @internal
     */
    CommandDispatcher _dispatcher;
};

#endif // COMMANDPROCESSOR_H
