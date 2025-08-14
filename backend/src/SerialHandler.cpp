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
            if (byte == '$')
            {
                index = 0;
                if (_readStringInput(byte))
                {
                    state = WAIT_FOR_START;
                }
                continue;
            }

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
                    // if (Utils::nonBlockingDelay(PACKET_LOG_INTERVAL_MS, lastPacketTime))
                    // {
                    //     Serial.print("Valid packets: ");
                    //     Serial.print(packetCount);
                    //     Serial.print("/sec | Total bytes: ");
                    //     Serial.println(packetCount * (4 + payloadLength + 4));
                    //     packetCount = 0;
                    // }

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

bool SerialHandler::_validateCRCAndEnd(std::array<uint8_t, MAX_PACKAGE_SIZE>& buffer, size_t totalLength, uint8_t payloadLength)
{
    // Calc correct position based on packet structure
    // START(3) + LEN(1) + PAYLOAD(payloadLength) + CRC(2) + END(2)
    size_t crcPos = 4 + payloadLength;
    size_t endPos = crcPos + 2;

    // Check if enough bytes
    if (totalLength < endPos + 2)
    {
        Serial.println("Not enough bytes received");
        return false;
    }

    // Check end bytes
    uint8_t endByte1      = buffer[endPos];
    uint8_t endByte2      = buffer[endPos + 1];
    bool    endBytesValid = (endByte1 == END_BYTES[0] && endByte2 == END_BYTES[1]);

    // Check crc
    uint8_t  crcLow      = buffer[crcPos];
    uint8_t  crcHigh     = buffer[crcPos + 1];
    uint16_t receivedCrc = (crcHigh << 8) | crcLow;

    // Calc crc over payload
    _crc.restart();
    for (size_t i = 4; i < 4 + static_cast<size_t>(payloadLength); i++)
    {
        _crc.add(buffer[i]);
    }
    uint16_t calculatedCrc = _crc.calc();

    bool crcValid = (receivedCrc == calculatedCrc);

    return endBytesValid && crcValid;
}

void SerialHandler::_forwardInput(const std::array<uint8_t, SerialHandler::MAX_PACKAGE_SIZE>& buffer, const uint8_t payloadLen, const size_t totalLength)
{
    std::vector<uint8_t> validPacket(buffer.begin(), buffer.begin() + totalLength);

    if (_commandProcessor)
    {

        _commandProcessor->processBinaryInput(validPacket, payloadLen);
    }
    else
    {
        Serial.println("Error: No CommandProcessor set. Please set one using setCommandProcessor() in Setup.cpp");
    }
}

// For Setup string input
bool SerialHandler::_readStringInput(const char startByte)
{
    static char   message[MAX_MESSAGE_SIZE];
    static size_t strIndex  = 0;
    static bool   receiving = false;

    while (Serial.available() > 0)
    {
        char c = Serial.read();

        if (!receiving)
        {
            receiving           = true;
            strIndex            = 0;         // Reset buffer
            message[strIndex++] = startByte; // set first byte
        }

        if (strIndex < MAX_MESSAGE_SIZE - 1)
        {
            message[strIndex++] = c;
            if (c == '#') // End of message
            {
                message[strIndex] = '\0';
                receiving         = false;

                String input = String(message);
                input.trim();

                if (_commandProcessor)
                {

                    _commandProcessor->processStringInput(input);
                }
                else
                {
                    Serial.println("Error: No CommandProcessor set. Please set one using setCommandProcessor() in Setup.cpp");
                }
                strIndex = 0;
                return true;
            }
        }
        else
        {
            // Buffer overflow
            receiving = false;
            strIndex  = 0;
            Serial.println("Error: Serial message too long");
            return true;
        }
    }
    return false;
}