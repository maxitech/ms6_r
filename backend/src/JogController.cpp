#include "JogController.h"
#include "Setup.h"

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
        Serial.println("Error: jogSpeeds vector too large");
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
            Serial.println("Error: Invalid motor index: " + String(index));
            return;
        }

        if (index >= static_cast<int>(_jogFlags.size()))
        {
            Serial.println("Error: jogFlags index out of bounds: " + String(index));
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
            if (Utils::nonBlockingDelay(100, lastSendTime))
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
            Serial.println("Error: Invalid index for JOG_START");
            return;
        }

        if (jogFlags->limitReached && direction == jogFlags->blockedDir && !jogFlags->runOnce)
        {
            Serial.println("Cannot jog in " + direction + " direction - limit reached");
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
                Serial.println("Invalid direction string: " + direction);
                return;
            }

            int dirVel = jogSpeedsValid[index];
            Serial.println(dirVel);
            _motorConfigs[index]->motor->rotateAsync(dirVel);
        }
        else if (currJogState == JOGGING && !jogFlags->limitReached)
        {
            std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);

            if (index >= static_cast<int>(jointAngles.size()))
            {
                Serial.println("Error: Joint angles index out of bounds");
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
            Serial.println("Warning: Resetting invalid _activeIndex: " + String(_activeIndex));
            _activeIndex = -1;
            currJogState = IDLE_JOG;
        }
        break;

    case JOG_UNKNOWN:
    default:
        Serial.println("Unknown JOG state.");
        break;
    }
}

String JogController::_getDir(std::vector<int32_t>& jogSpeedsValid, int index)
{
    if (index < 0 || index >= static_cast<int>(jogSpeedsValid.size()))
    {
        Serial.println("Error: Invalid index in _getDir: " + String(index));
        return "";
    }

    return (jogSpeedsValid[index] >= 0) ? "POS" : "NEG";
}