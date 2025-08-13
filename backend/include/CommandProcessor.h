/**
 * @file CommandProcessor.h
 * @brief Declares the CommandProcessor class responsible for processing and dispatching incoming serial commands.
 * @details This class is a core component of the robot control system, responsible for parsing serial input and delegating commands for execution.
 */

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "ComProtocoll.h"
#include "CommandDispatcher.h"
#include "ProcessedData.h"
#include "SerialHandler.h"
#include <Arduino.h>
#include <vector>

/**
 * @class CommandProcessor
 * @brief Handles input commands and processes them.
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
     * @brief Processes an input buffer and forwards the data.
     * @param buffer Vector which holds one valid packet at the time.
     * @param payloadLen The exact length of the whole  payload.
     */
    void processBinaryInput(const std::vector<uint8_t>& buffer, const uint8_t payloadLen);

    /**
     * @brief Processes an input string, validates it, and if all valid, it forwards the data.
     * @param input The input string in the format $<cmd>*<checksum>#.
     */
    void processStringInput(const String& input);

private:
    // ************* Binary **************
    /**
     * @brief Decodes a byte stream into signed 24-bit integers.
     *
     * This function interprets the input byte vector as a sequence of big-endian
     * signed 24-bit integers (3 bytes per value). Sign-extension is applied to
     * ensure correct conversion to 32-bit signed integers.
     *
     * @param data The input byte stream, where each value consists of 3 bytes.
     * @return A vector of decoded int32_t values.
     */
    std::vector<int32_t> _decodeSigned24BitValues(const std::vector<uint8_t>& data);

    /**
     * @brief Processes a valid command.
     * @param cmd The data string (after validation).
     * @internal
     */
    // void _processCommand(const String& cmd);

    // ************* Sring **************
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

    ProcessedData _processedData;
};

#endif // COMMANDPROCESSOR_H
