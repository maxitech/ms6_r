#include "PacketBuilder.h"

using namespace CommunicationProtocoll;

CRC16 PacketBuilder::_crc;

PacketBuilder::PacketBuilder()
{
}

std::vector<uint8_t> PacketBuilder::buildPacket(const std::vector<uint8_t>& payload)
{
    std::vector<uint8_t> packet;

    // push start bytes
    for (uint8_t b : START_BYTES)
    {
        packet.push_back(b);
    }

    uint8_t payloadLen = static_cast<uint8_t>(payload.size());
    packet.push_back(payloadLen);

    packet.insert(packet.end(), payload.begin(), payload.end());

    // Calc crc over payload incl. cmdId
    _crc.restart();
    for (size_t i = 4; i < 4 + static_cast<size_t>(payloadLen); i++)
    {
        _crc.add(packet[i]);
    }
    uint16_t calculatedCrc = _crc.calc();
    auto     crcBytes      = _uint16CrcToBytesLsb(calculatedCrc);
    packet.insert(packet.end(), crcBytes.begin(), crcBytes.end());

    for (uint8_t b : END_BYTES)
    {
        packet.push_back(b);
    }

    return packet;
}

std::vector<uint8_t> PacketBuilder::buildResponsePayload(const uint8_t cmdId, const uint8_t status, const uint8_t detail)
{
    std::vector<uint8_t> responsePayload;
    responsePayload.insert(responsePayload.begin(), cmdId);
    responsePayload.insert(responsePayload.end(), status);
    responsePayload.insert(responsePayload.end(), detail);

    return responsePayload;
}

std::vector<uint8_t> PacketBuilder::buildResponsePayload(const uint8_t cmdId, const uint8_t status, const uint8_t detail, std::vector<uint8_t> data)
{
    std::vector<uint8_t> responsePayload;
    responsePayload.insert(responsePayload.begin(), cmdId);
    responsePayload.insert(responsePayload.end(), status);
    responsePayload.insert(responsePayload.end(), detail);
    responsePayload.insert(responsePayload.end(), data.begin(), data.end());

    return responsePayload;
}

std::vector<uint8_t> PacketBuilder::_uint16CrcToBytesLsb(uint16_t crcVal)
{
    std::vector<uint8_t> crcBytes;
    uint8_t              crcLsb = crcVal & 0xFF;
    uint8_t              crcMsb = (crcVal >> 8) & 0xFF;
    crcBytes.push_back(crcLsb);
    crcBytes.push_back(crcMsb);
    return crcBytes;
}
