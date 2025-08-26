#include "RobotDataSender.h"
#include "ComProtocol.h"
#include "PacketBuilder.h"
#include "Setup.h"
#include "Utils.h"

using namespace CommunicationProtocoll;

RobotDataSender::RobotDataSender() {};

void RobotDataSender::sendMotorPosInSteps(const std::vector<MotorConfig*>& motorCoinfigs)
{
    uint8_t              cmdId = SYS_DATA_REQUEST;
    std::vector<uint8_t> data;

    for (size_t i = 0; i < motorCoinfigs.size(); ++i)
    {
        int32_t pos = 0;
        if (_homedMask & (1 << i))
        {
            pos = motorCoinfigs[i]->motor->getPosition();
        }
        data.push_back((pos >> 16) & 0xFF); // MSB
        data.push_back((pos >> 8) & 0xFF);
        data.push_back(pos & 0xFF); // LSB
    }
    Utils::createAndSendPacket(cmdId, STATUS_OK, DATA_STEPS, data);
}
