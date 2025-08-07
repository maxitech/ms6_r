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
    int                  payloadBegin = 4;
    std::vector<uint8_t> payload(packet.begin() + payloadBegin, packet.begin() + (payloadBegin + payloadLen));

    uint8_t fixedPayloadLen;
    if (!payload.empty())
    {
        fixedPayloadLen = payload.back();
    }

    const uint8_t cmdId = payload[0];

    std::vector<uint8_t> jogSpeedBytes;
    std::vector<int32_t> jogSpeeds;

    size_t               fixPlLenByte = 1;
    std::vector<uint8_t> fixedPayload(payload.begin() + (payloadLen - fixPlLenByte) - fixedPayloadLen, payload.end() - 1);

    switch (cmdId)
    {
    case CMD_JOG:
        // decode jog speeds
        jogSpeedBytes = std::vector<uint8_t>(payload.begin() + 1, payload.begin() + 1 + 18);
        jogSpeeds     = _decodeValues(jogSpeedBytes);
        break;
    default:
        break;
    }

    // _processCommand(data);
}

std::vector<int32_t> CommandProcessor::_decodeValues(const std::vector<uint8_t>& data)
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