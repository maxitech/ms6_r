#include "Utils.h"
#include "PacketBuilder.h"

Utils::Utils()
{
}

bool Utils::nonBlockingDelay(unsigned long  interval,
                             unsigned long& lastUpdateTime)
{
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void Utils::createAndSendPacket(const uint8_t cmdId, const uint8_t status, const uint8_t detail)
{
    std::vector<uint8_t> payload = PacketBuilder::buildResponsePayload(cmdId, status, detail);
    std::vector<uint8_t> packet  = PacketBuilder::buildPacket(payload);
    Serial.write(packet.data(), packet.size());
    Serial.flush();
}

void Utils::createAndSendPacket(const uint8_t cmdId, const uint8_t status, const uint8_t detail, std::vector<uint8_t> data)
{
    std::vector<uint8_t> payload = PacketBuilder::buildResponsePayload(cmdId, status, detail, data);
    std::vector<uint8_t> packet  = PacketBuilder::buildPacket(payload);
    Serial.write(packet.data(), packet.size());
    Serial.flush();
}