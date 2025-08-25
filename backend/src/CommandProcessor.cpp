#include "CommandProcessor.h"
#include "ComProtocol.h"
#include "DebugLog.h"
#include "Utils.h"

#define LOG(level, msg) DebugLog::log(level, msg)

using namespace CommunicationProtocoll;

CommandProcessor::CommandProcessor(ProgramLoader& programLoader)
    : _dispatcher(programLoader)
{
}

// ************** Binary ***************
void CommandProcessor::processBinaryInput(const std::vector<uint8_t>& packet, uint8_t payloadLen)
{
    // Payload
    int                  payloadBegin = 4;
    std::vector<uint8_t> payload(packet.begin() + payloadBegin, packet.begin() + (payloadBegin + payloadLen));

    const uint8_t cmdId  = payload[0];
    _processedData.cmdId = cmdId; //! add validation later to early return if something is wrong with cmdId

    // Decode data out of payload based on cmdId
    switch (cmdId)
    {
    case CMD_LOAD:
    {
        uint8_t program        = payload[1];
        _processedData.program = program;
        break;
    }
    case CMD_JOG:
    {

        // decode jog speeds
        std::vector<uint8_t> jogSpeedBytes(payload.begin() + 1, payload.begin() + 1 + 18);
        std::vector<int32_t> jogSpeeds = _decodeSigned24BitValues(jogSpeedBytes);
        _processedData.jogSpeeds       = jogSpeeds;
        break;
    }
    default:
        break;
    }

    // Fix Payload
    uint8_t fixedPayloadLen;
    if (!payload.empty())
    {
        fixedPayloadLen = payload.back();
    }
    size_t               fixPlLenByte = 1;
    std::vector<uint8_t> fixedPayload(payload.begin() + (payloadLen - fixPlLenByte) - fixedPayloadLen, payload.end() - 1);
    uint8_t              telemetryByte = fixedPayload[0];
    if (telemetryByte == 0x01)
    {
        _processedData.is_requestTelemetry = true;
    }
    else if (telemetryByte == 0x00)
    {
        _processedData.is_requestTelemetry = false;
    }
    else
    {
        _processedData.is_requestTelemetry = std::nullopt;
    }

    if (!_processedData.is_requestTelemetry.has_value())
    {
        Utils::createAndSendPacket(cmdId, STATUS_ERROR, ERR_INVALID_TELEMETRY_FLAG);
        // Debug log
        LOG(LOG_WARN, "Invalid telemetry flag received");
        return;
    }

    // forward processed data struct to
    _dispatcher.dispatch(_processedData);
}

std::vector<int32_t> CommandProcessor::_decodeSigned24BitValues(const std::vector<uint8_t>& data)
{
    std::vector<int32_t> result;
    for (size_t i = 0; i + 2 < data.size(); i += 3)
    {
        int32_t value = (static_cast<int32_t>(data[i + 0]) << 16) |
                        (static_cast<int32_t>(data[i + 1]) << 8) |
                        (static_cast<int32_t>(data[i + 2]) << 0);

        if (value & 0x800000)
        {
            value |= 0xFF000000;
        }

        result.push_back(value);
    }
    return result;
}

// ************** String ***************
void CommandProcessor::processStringInput(const String& input)
{
    if (!_isInputValid(input))
    {
        Utils::createAndSendPacket(CMD_SETUP, STATUS_ERROR, ERR_INVALID_PACKET);
        // Debug log
        LOG(LOG_ERROR, "Input is invalid. Expected input: $<cmd_pt1, cmd_pt2, etc.>*<checksum>#");
        return;
    }

    int    delimiterIndex = input.indexOf("*");
    String data           = input.substring(1, delimiterIndex);
    String checksum       = input.substring(delimiterIndex + 1, input.length() - 1);

    if (!_validateChecksum(data, checksum))
    {
        Utils::createAndSendPacket(CMD_SETUP, STATUS_ERROR, ERR_CHECKSUM);
        // Debug log
        LOG(LOG_ERROR, "Input processing failed: Checksum validation error.");
        return;
    }

    _processCommand(data);
}

bool CommandProcessor::_isInputValid(const String& input)
{
    if (!input.startsWith("$") || !input.endsWith("#") || input.indexOf("*") == -1)
        return false;
    return true;
}

bool CommandProcessor::_validateChecksum(const String& data, const String& checksum)
{
    uint8_t checksumValue = 0;

    // Calculate checksum by XORing all characters in data -> (8-bit checksum)
    for (size_t i = 0; i < data.length(); i++)
    {
        checksumValue ^= static_cast<uint8_t>(data[i]);
    }

    String calculatedChecksum = String(checksumValue, HEX).toUpperCase();
    if (calculatedChecksum.length() == 1)
    {
        calculatedChecksum = "0" + calculatedChecksum;
    }

    if (calculatedChecksum != checksum)
    {

        Utils::createAndSendPacket(CMD_SETUP, STATUS_ERROR, ERR_CHECKSUM);
        // Debug log
        String debugMsg = "Checksum Error. Expected: " + checksum + ", Calculated: " + calculatedChecksum;
        LOG(LOG_ERROR, debugMsg);
        return false;
    }

    return true;
}

void CommandProcessor::_processCommand(const String& cmd)
{
    std::pair<String, std::vector<String>> parts = _splitString(cmd);

    _dispatcher.dispatch(parts.first, parts.second);
}

std::pair<String, std::vector<String>> CommandProcessor::_splitString(const String& str)
{
    std::vector<String> tokens;
    int                 firstComma = str.indexOf(',');
    if (firstComma == -1)
    {
        return {str, tokens};
    }

    String command  = str.substring(0, firstComma);
    String paramStr = str.substring(firstComma + 1);

    if (!paramStr.startsWith("[") || !paramStr.endsWith("]"))
    {
        Utils::createAndSendPacket(CMD_SETUP, STATUS_ERROR, ERR_INVALID_PACKET);
        // Debug log
        LOG(LOG_ERROR, "Command invalid! Correct format <cmd,[arg, arg, ...]>");
        return {str, tokens};
    }

    paramStr = paramStr.substring(1, paramStr.length() - 1); // Remove outer brackets

    // JSON-aware splitting:
    String current;
    int    braceDepth   = 0; // {}
    int    bracketDepth = 0; // []

    for (unsigned int i = 0; i < paramStr.length(); ++i)
    {
        char c = paramStr[i];
        if (c == '{')
            braceDepth++;
        else if (c == '}')
            braceDepth--;
        else if (c == '[')
            bracketDepth++;
        else if (c == ']')
            bracketDepth--;

        if (c == ',' && braceDepth == 0 && bracketDepth == 0)
        {
            tokens.push_back(current);
            current = "";
        }
        else
        {
            current += c;
        }
    }

    if (current.length() > 0)
    {
        tokens.push_back(current);
    }

    return {command, tokens};
}