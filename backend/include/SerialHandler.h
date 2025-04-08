/**
 * @file SerialHandler.h
 * @brief Declares the SerialHandler class responsible for receiving serial input and delegating it to the CommandProcessor.
 * @details This class monitors the serial interface, reads incoming data, and passes valid strings to the command processor.
 *          It is designed to be lightweight and modular, allowing flexible integration with other system components.
 */

#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

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

private:
    /**
     * @brief Reads data from the serial buffer and trims whitespace.
     * @internal
     */
    void _readSerialInput();

    /**
     * @brief Forwards the input string to the CommandProcessor, if available.
     * @param input The trimmed input string from the serial port.
     * @internal
     */
    void _forwardInput(const String& input);

    /**
     * @brief Pointer to the CommandProcessor used for input handling.
     */
    CommandProcessor* _commandProcessor = nullptr;
};

#endif // SERIAL_HANDLER_H
