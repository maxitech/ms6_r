#include "JogController.h"
#include "ComProtocol.h"
#include "DebugLog.h"
#include "Setup.h"
#include "Utils.h"

using namespace CommunicationProtocoll;

#define LOG(level, msg) DebugLog::log(level, msg)

JogController::JogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender)
    , _activeIndex(-1) {};

bool JogController::jogJoint(std::optional<std::vector<int32_t>>& jogSpeeds)
{
    if (!jogSpeeds.has_value() || jogSpeeds->empty())
        return false;

    std::vector<int32_t> jogSpeedsValid = jogSpeeds.value();

    if (jogSpeedsValid.size() > _motorConfigs.size())
    {
        Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
        LOG(LOG_ERROR, "Vector 'jogSpeeds' too large.");
        return false;
    }

    bool                anyActive   = false;
    std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);
    for (size_t i = 0; i < jogSpeedsValid.size(); ++i)
    {
        JogFlags* jogFlags  = &_jogFlags[i];
        String    direction = _getDir(jogSpeedsValid, i);

        float currPosDeg = jointAngles[i];

        // Check limits
        if (jogFlags->limitReached && direction == jogFlags->blockedDir && jogSpeedsValid[i] != 0)
        {
            Utils::createAndSendPacket(CMD_JOG, STATUS_OK, WARN_LIMIT_HIT);
            String debugMsg = "Cannot jog in '" + direction + "' direction - limit reached.";
            LOG(LOG_INFO, debugMsg);
            return false;
        }

        // Reset flags
        if (jogFlags->limitReached && direction != jogFlags->blockedDir)
        {
            jogFlags->limitReached = false;
            jogFlags->blockedDir   = "";
        }

        // Limit reached
        if ((currPosDeg >= _motorConfigs[i]->maxAngleDeg && jogSpeedsValid[i] > 0) || (currPosDeg <= _motorConfigs[i]->minAngleDeg && jogSpeedsValid[i] < 0))
        {
            _motorConfigs[i]->motor->setSpeed(0);
            jogFlags->limitReached = true;
            jogFlags->blockedDir   = direction;
            Utils::createAndSendPacket(CMD_JOG, STATUS_OK, WARN_LIMIT_HIT);
            continue;
        }

        // Start jogging
        if (!jogFlags->limitReached && jogSpeedsValid[i] != 0)
        {
            // start
            if (direction != "POS" && direction != "NEG")
            {
                Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INVALID_DIR);
                String debugMsg = "Invalid direction string: '" + direction + "'.";
                LOG(LOG_ERROR, debugMsg);
                return false;
            }

            _motorConfigs[i]->motor->setSpeed(jogSpeedsValid[i]);
            _motorConfigs[i]->motor->runSpeed();
            anyActive = true;
        }
    }

    // Send periodic updates if any motor is active
    static unsigned long lastSendTime = 0;
    if (Utils::nonBlockingDelay(50, lastSendTime))
    {
        _rbtDtaSender.sendMotorPosInSteps(_motorConfigs);
    }
    return anyActive;
}

String JogController::_getDir(std::vector<int32_t>& jogSpeedsValid, int index)
{
    if (index < 0 || index >= static_cast<int>(jogSpeedsValid.size()))
    {
        Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
        String debugMsg = "Invalid index in '_getDir': '" + String(index) + "'.";
        LOG(LOG_ERROR, debugMsg);

        return "";
    }

    return (jogSpeedsValid[index] >= 0) ? "POS" : "NEG";
}