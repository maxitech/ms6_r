/**
 * @file SerialHandler.h
 * @brief Declares the SerialHandler class responsible for receiving serial input and delegating it to the CommandProcessor.
 * @details This class monitors the serial interface, reads incoming data, and passes valid strings to the command processor.
 *          It is designed to be lightweight and modular, allowing flexible integration with other system components.
 */

#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include "CRC16.h"
#include "ComProtocol.h"
#include <Arduino.h>

class CommandProcessor;

/**
 * @class SerialHandler
 * @brief Handles incoming serial communication and passes data to a linked CommandProcessor.
 */
class SerialHandler
{
public:
    /**
     * @brief Constructs a SerialHandler instance.
     */
    SerialHandler();

    /**
     * @brief Listens for incoming serial data and processes it.
     * @note This should be called regularly in the main loop to ensure timely data handling.
     */
    void listenForSerial();

    /**
     * @brief Assigns a CommandProcessor to handle validated input.
     * @param processor Pointer to a CommandProcessor instance.
     */
    void setCommandProcessor(CommandProcessor* processor);

    /**
     * @brief Maximum size of the serial data buffer.
     */
    static constexpr size_t MAX_PACKAGE_SIZE = 256;

private:
    /**
     * @brief Reads bytes from serial and handles validation.
     * @internal
     */
    void _readSerialInput();

    /**
     * @brief Reads data from the serial buffer and trims whitespace.
     * @internal
     */
    bool _readStringInput(const char startByte);

    /**
     * @brief Forwards the input string to the CommandProcessor, if available.
     * @param input The trimmed input string from the serial port.
     * @internal
     */
    void _forwardInput(const std::array<uint8_t, SerialHandler::MAX_PACKAGE_SIZE>& buffer, const uint8_t payloadLen, const size_t totalLength);

    /**
     * @brief Pointer to the CommandProcessor used for input handling.
     */
    CommandProcessor*       _commandProcessor = nullptr;
    uint8_t                 _cmdId            = NOP;
    bool                    _validateCRCAndEnd(std::array<uint8_t, MAX_PACKAGE_SIZE>& buffer, size_t index, uint8_t payloadLength);
    CRC16                   _crc;
    static constexpr size_t MAX_MESSAGE_SIZE = 2048;
};

#endif // SERIAL_HANDLER_H
