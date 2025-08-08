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
    void processInput(const std::vector<uint8_t>& buffer, const uint8_t payloadLen);

private:
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

    /**
     * @brief Dispatcher used to handle commands internally.
     * @note Initialized with a reference to the ProgramLoader instance passed to the constructor.
     * @internal
     */
    CommandDispatcher _dispatcher;

    ProcessedData _processedData;
};

#endif // COMMANDPROCESSOR_H
