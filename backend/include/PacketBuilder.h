#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H

#include "CRC16.h"
#include "ComProtocoll.h"

class PacketBuilder
{
public:
    static std::vector<uint8_t> buildPacket(uint8_t cmdId, const std::vector<uint8_t>& payload);

private:
    static std::vector<uint8_t> _uint16CrcToBytesLsb(uint16_t value);
    static CRC16                _crc;
};

#endif // PACKETBUILDER_H