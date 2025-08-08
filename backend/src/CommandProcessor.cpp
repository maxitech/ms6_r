#include "CommandProcessor.h"
#include "ComProtocoll.h"
#include "Utils.h"

using namespace CommunicationProtocoll;

CommandProcessor::CommandProcessor(ProgramLoader& programLoader)
    : _dispatcher(programLoader)
{
}

void CommandProcessor::processInput(const std::vector<uint8_t>& packet, uint8_t payloadLen)
{
    // Payload
    int                  payloadBegin = 4;
    std::vector<uint8_t> payload(packet.begin() + payloadBegin, packet.begin() + (payloadBegin + payloadLen));

    const uint8_t cmdId = payload[0];
    processedData.cmdId = cmdId; //! add validation later to early return if something is wrong with cmdId

    // Decode data out of payload based on cmdId
    switch (cmdId)
    {
    case CMD_JOG:
    {

        // decode jog speeds
        std::vector<uint8_t> jogSpeedBytes(payload.begin() + 1, payload.begin() + 1 + 18);
        std::vector<int32_t> jogSpeeds = _decodeSigned24BitValues(jogSpeedBytes);
        processedData.jogSpeeds        = jogSpeeds;
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
        processedData.is_requestTelemetry = true;
    }
    else if (telemetryByte == 0x00)
    {
        processedData.is_requestTelemetry = false;
    }
    else
    {
        processedData.is_requestTelemetry = std::nullopt;
    }

    if (!processedData.is_requestTelemetry.has_value())
    {
        Serial.println("Warning: Invalid telemetry flag received");
        return;
    }
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