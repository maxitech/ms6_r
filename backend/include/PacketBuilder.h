#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H

#include "CRC16.h"
#include "ComProtocol.h"

class PacketBuilder
{
public:
    PacketBuilder();
    static std::vector<uint8_t> buildPacket(const std::vector<uint8_t>& payload);
    static std::vector<uint8_t> buildResponsePayload(const uint8_t cmdId, const uint8_t status, const uint8_t detail);
    static std::vector<uint8_t> buildResponsePayload(const uint8_t cmdId, const uint8_t status, const uint8_t detail, std::vector<uint8_t> data);

private:
    static std::vector<uint8_t> _uint16CrcToBytesLsb(uint16_t value);
    static CRC16                _crc;
};

#endif // PACKETBUILDER_H