#include "SerialHandler.h"
#include "CommandProcessor.h"

SerialHandler::SerialHandler() {}

void SerialHandler::setCommandProcessor(CommandProcessor* processor)
{
    _commandProcessor = processor;
}

void SerialHandler::listenForSerial()
{
    _readSerialInput();
}

void SerialHandler::_readSerialInput()
{
    static enum ParserState {
        WAIT_FOR_START,
        READ_LENGTH,
        READ_PAYLOAD,
        READ_CRC,
        READ_END
    } state = WAIT_FOR_START;

    static uint8_t buffer[MAX_PACKAGE_SIZE];
    static size_t  index         = 0;
    static uint8_t payloadLength = 0;

    while (Serial.available() > 0)
    {
        uint8_t byte = Serial.read();

        switch (state)
        {
        case WAIT_FOR_START:
            buffer[index++] = byte;
            if (index >= 3 &&
                buffer[index - 3] == START_BYTES[0] &&
                buffer[index - 2] == START_BYTES[1] &&
                buffer[index - 1] == START_BYTES[2])
            {
                index = 3; // Start of actual payload (next byte will be LEN)
                state = READ_LENGTH;
            }
            else if (index > 3)
            // Sliding window: keep last 2 bytes, write new byte at buffer[2]
            {
                buffer[0] = buffer[index - 2];
                buffer[1] = buffer[index - 1];
                buffer[2] = byte;
                index     = 3;
            }
            break;
        case READ_LENGTH:
            payloadLength   = byte;
            buffer[index++] = byte;
            // if buffer size not packet len -> timeout
            state = READ_PAYLOAD;
            break;
        case READ_PAYLOAD:
            buffer[index++] = byte;
            if (index >= 4 + static_cast<size_t>(payloadLength))
            {
                state = READ_CRC;
            }
            break;
        case READ_CRC:
            buffer[index++] = byte;
            if (index >= 4 + static_cast<size_t>(payloadLength) + 2) // 2 bytes crc
            {
                state = READ_END;
            }
            break;

        case READ_END:
            buffer[index++] = byte;
            if (index >= 4 + static_cast<size_t>(payloadLength) + 4) // last 2 end bytes
            {
                // validate crc and end bytes
                // process payload
                state = WAIT_FOR_START;
                index = 0;
            }
            break;
        default:
            Serial.println("Unexpected error in 'SerialHandler.cpp'");
            break;
        }
    }
}

void SerialHandler::_forwardInput(const String& input)
{
    if (_commandProcessor)
    {
        _commandProcessor->processInput(input);
    }
    else
    {
        Serial.println("Error: No CommandProcesor set. Please set one using setCommandProcessor() in Setup.cpp");
    }
}