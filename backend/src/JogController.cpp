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

void JogController::jogJoint(std::optional<std::vector<int32_t>>& jogSpeeds, JogState& currJogState)
{
    if (!jogSpeeds.has_value())
    {
        return;
    }
    std::vector<int32_t> jogSpeedsValid = jogSpeeds.value();

    if (jogSpeedsValid.empty())
    {
        return;
    }

    if (jogSpeedsValid.size() > _motorConfigs.size())
    {
        Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
        LOG(LOG_ERROR, "Vector 'jogSpeeds' too large.");
        return;
    }

    int index = -1;
    for (size_t i = 0; i < jogSpeedsValid.size(); ++i)
    {
        if (jogSpeedsValid[i] != 0)
        {
            index = static_cast<int>(i);
            break;
        }
    }

    JogCommand jogCmd;
    String     direction;
    JogFlags*  jogFlags = nullptr;

    if (index == -1)
    {
        jogCmd    = JOG_STOP;
        direction = "";
    }
    else
    {
        if (index >= static_cast<int>(_motorConfigs.size()) || index < 0)
        {
            Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
            String debugMsg = "Invalid motor index: " + String(index) + ".";
            LOG(LOG_ERROR, debugMsg);
            return;
        }

        if (index >= static_cast<int>(_jogFlags.size()))
        {
            Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
            String debugMsg = "'jogFlags' index out of bounds: " + String(index) + ".";
            LOG(LOG_ERROR, debugMsg);
            return;
        }

        jogCmd       = JOG_START;
        direction    = _getDir(jogSpeedsValid, index);
        _activeIndex = index;
        jogFlags     = &_jogFlags[index];
    }

    if (currJogState == JOGGING)
    {
        if (_activeIndex >= 0 && _activeIndex < static_cast<int>(_motorConfigs.size()))
        {
            static unsigned long lastSendTime = 0;
            if (Utils::nonBlockingDelay(5, lastSendTime))
            {
                _rbtDtaSender.sendMotorPosInSteps(_motorConfigs);
            }
        }
    }

    switch (jogCmd)
    {
    case JOG_START:
        if (!jogFlags)
        {
            return;
        }

        if (index < 0 || index >= static_cast<int>(_motorConfigs.size()))
        {
            Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
            LOG(LOG_ERROR, "Invalid index for 'JOG_START'.");
            return;
        }

        if (jogFlags->limitReached && direction == jogFlags->blockedDir && !jogFlags->runOnce)
        {
            Utils::createAndSendPacket(CMD_JOG, STATUS_OK, WARN_LIMIT_HIT);
            String debugMsg = "Cannot jog in '" + direction + "' direction - limit reached.";
            LOG(LOG_INFO, debugMsg);
            jogFlags->runOnce = true;
            return;
        }

        if (jogFlags->limitReached && direction != jogFlags->blockedDir)
        {
            jogFlags->limitReached = false;
            jogFlags->runOnce      = false;
            jogFlags->blockedDir   = "";
        }

        if (currJogState != JOGGING && !jogFlags->limitReached)
        {
            // start
            currJogState = JOGGING;

            if (direction != "POS" && direction != "NEG")
            {
                Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INVALID_DIR);
                String debugMsg = "Invalid direction string: '" + direction + "'.";
                LOG(LOG_ERROR, debugMsg);
                return;
            }

            int dirVel = jogSpeedsValid[index];
            _motorConfigs[index]->motor->rotateAsync(dirVel);
        }
        else if (currJogState == JOGGING && !jogFlags->limitReached)
        {
            std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);

            if (index >= static_cast<int>(jointAngles.size()))
            {
                Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_INDEX_OOB);
                LOG(LOG_ERROR, "Joint angles index out of bounds.");
                return;
            }

            float currPosDeg = jointAngles[index];

            if ((currPosDeg <= (_motorConfigs[index]->maxAngleDeg)) ||
                (currPosDeg >= (_motorConfigs[index]->minAngleDeg)))
            {
                _motorConfigs[index]->motor->emergencyStop();

                currJogState           = IDLE_JOG;
                jogFlags->limitReached = true;
                jogFlags->blockedDir   = direction;

                _rbtDtaSender.sendMotorPosInSteps(_motorConfigs);
            }
        }
        break;

    case JOG_STOP:
        if (_activeIndex >= 0 &&
            _activeIndex < static_cast<int>(_motorConfigs.size()) &&
            _activeIndex < static_cast<int>(_jogFlags.size()) &&
            currJogState == JOGGING)
        {
            JogFlags& activeFlags = _jogFlags[_activeIndex];

            if (!activeFlags.limitReached)
            {
                // _motorConfigs[_activeIndex]->motor->stopAsync();
                _motorConfigs[_activeIndex]->motor->emergencyStop();
                currJogState = IDLE_JOG;

                _rbtDtaSender.sendMotorPosInSteps(_motorConfigs);
            }
            _activeIndex = -1;
        }
        else if (_activeIndex != -1)
        {
            _activeIndex = -1;
            currJogState = IDLE_JOG;
        }
        break;

    case JOG_UNKNOWN:
    default:
        Utils::createAndSendPacket(CMD_JOG, STATUS_ERROR, ERR_UNKNOWN);
        LOG(LOG_ERROR, "Unknown JOG state.");
        break;
    }
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