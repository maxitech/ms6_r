#include "SerialHandler.h"
#include "CommandProcessor.h"
#include "Utils.h"

using namespace CommunicationProtocoll;

SerialHandler::SerialHandler()
    : _crc(CRC16_MODBUS_POLYNOME,
           CRC16_MODBUS_INITIAL,
           CRC16_MODBUS_XOR_OUT,
           CRC16_MODBUS_REV_IN,
           CRC16_MODBUS_REV_OUT)
{
}

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

    static std::array<uint8_t, MAX_PACKAGE_SIZE> buffer {};
    static size_t                                index          = 0;
    static uint8_t                               payloadLength  = 0;
    static unsigned long                         lastByteTime   = 0;
    static unsigned long                         lastPacketTime = 0;
    static uint32_t                              packetCount    = 0;

    const unsigned long BYTE_TIMEOUT_MS        = 50;
    const unsigned long PACKET_LOG_INTERVAL_MS = 1000;

    while (Serial.available() > 0)
    {
        uint8_t byte = Serial.read();
        lastByteTime = millis();

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
            {
                // Sliding window: keep last 2 bytes, write new byte at buffer[2]
                buffer[0] = buffer[index - 2];
                buffer[1] = buffer[index - 1];
                buffer[2] = byte;
                index     = 3;
            }
            break;

        case READ_LENGTH:
            payloadLength   = byte;
            buffer[index++] = byte;
            state           = READ_PAYLOAD;
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
                bool is_valid = _validateCRCAndEnd(buffer, index, payloadLength);

                if (is_valid)
                {
                    packetCount++;

                    // Debug log only after intervall -> better performance
                    if (Utils::nonBlockingDelay(PACKET_LOG_INTERVAL_MS, lastPacketTime))
                    {
                        Serial.print("Valid packets: ");
                        Serial.print(packetCount);
                        Serial.print("/sec | Total bytes: ");
                        Serial.println(packetCount * (4 + payloadLength + 4));
                        packetCount = 0;
                    }

                    _forwardInput(buffer, payloadLength, index);
                }
                else
                {
                    Serial.println("Invalid packet - discarding");
                }

                // reset for next packet
                state = WAIT_FOR_START;
                index = 0;
                return;
            }
            break;

        default:
            Serial.println("Unexpected error in 'SerialHandler.cpp'");
            state = WAIT_FOR_START;
            index = 0;
            break;
        }
    }

    // Timeout check: If no bytes are received for a while during packet reception,
    // reset the parser state to avoid getting stuck due to an incomplete packet.
    if (state != WAIT_FOR_START && (millis() - lastByteTime) > BYTE_TIMEOUT_MS)
    {
        Serial.println("Packet timeout - resetting parser");
        state = WAIT_FOR_START;
        index = 0;
    }
}

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