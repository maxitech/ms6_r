#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H


#include <Arduino.h>


#include "SerialHandler.h"


/**
 * @brief Handles input commands and processes them, including validation and checksum verification.
 */
class CommandProcessor {
public:
    /**
     * @brief Constructor for the CommandProcessor class.
     */
    CommandProcessor();

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
     */
    bool _isInputValid(const String& input);

     /**
     * @brief Verifies the checksum of the command string.
     * @param data The portion of the string between `$` and `*`.
     * @param checksum The checksum value to validate against.
     * @return True if the checksum is correct, false otherwise.
     * @note The checksum is calculated using an XOR operation over all characters in `data`.
     */
    bool _validateChecksum(const String& data, const String& checksum);

    /**
     * @brief Processes a valid command.
     * @param cmd The data string (after validation).
     */
    void _processCommand(const String& cmd);

     /**
     * @brief Splits a string into parts based on a delimiter.
     * @param str The string to split.
     * @param delimiter The character used to separate the string into parts.
     * @return A vector of substrings.
     */
    std::vector<String> _splitString(const String& str, const char delimiter);
};


#endif // COMMANDPROCESSOR_H
