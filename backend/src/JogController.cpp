#include "JogController.h"
#include "Setup.h"

JogController::JogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender) {};

void JogController::jogJoint(std::vector<String>& arguments, JogState& currJogState, const int motorIdx)
{

    String&       direction = arguments[1];
    const int     velocity  = arguments[2].toInt();
    const String& jogState  = arguments.back();
    JogFlags&     jogFlags  = _jogFlags[motorIdx];

    JogCommand jogCmd = _getJogCommand(jogState);

    if (currJogState == JOGGING)
    {
        static unsigned long lastSendTime = 0;
        if (Utils::nonBlockingDelay(100, lastSendTime))
        {
            // Send motor position in steps
            _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[motorIdx], motorIdx);
            // Send forward kinematics pose and joint angles
            _rbtDtaSender.sendFkPoseAndJointAngles();
        }
    }

    switch (jogCmd)
    {
    case JOG_START:
        if (jogFlags.limitReached && direction == jogFlags.blockedDir && !jogFlags.runOnce)
        {
            Serial.println("Cannot jog in " + direction + " direction - limit reached");
            jogFlags.runOnce = true;
            return;
        }

        if (jogFlags.limitReached && direction != jogFlags.blockedDir)
        {
            jogFlags.limitReached = false;
            jogFlags.runOnce      = false;
            jogFlags.blockedDir   = "";
        }

        if (currJogState != JOGGING && !jogFlags.limitReached)
        {
            // start
            currJogState = JOGGING;

            if (direction != "POS" && direction != "NEG")
            {
                Serial.println("Invalid direction string: " + direction);
                return;
            }

            int dir    = (direction == "POS") ? 1 : -1;
            int dirVel = velocity * dir;

            _motorConfigs[motorIdx]->motor->rotateAsync(dirVel);
        }
        else if (currJogState == JOGGING && !jogFlags.limitReached)
        {
            std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);
            float               currPosDeg  = jointAngles[motorIdx];
            if (currPosDeg >= _motorConfigs[motorIdx]->minAngleDeg ||
                currPosDeg <= _motorConfigs[motorIdx]->maxAngleDeg)
            {
                _motorConfigs[motorIdx]->motor->emergencyStop();
                currJogState          = IDLE_JOG;
                jogFlags.limitReached = true;
                jogFlags.blockedDir   = direction;

                _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[motorIdx], motorIdx);
                _rbtDtaSender.sendFkPoseAndJointAngles();
            }
        }
        break;

    case JOG_STOP:
        if (currJogState == JOGGING && !jogFlags.limitReached)
        {
            // stop
            _motorConfigs[motorIdx]->motor->emergencyStop();
            currJogState = IDLE_JOG;

            _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[motorIdx], motorIdx);
            _rbtDtaSender.sendFkPoseAndJointAngles();
        }
        break;

    case JOG_UNKNOWN:
    default:
        Serial.println("Unknown JOG state: " + jogState);
        break;
    }
}

JogCommand JogController::_getJogCommand(const String& str)
{
    if (str == "START")
        return JOG_START;
    else if (str == "STOP")
        return JOG_STOP;
    else
        return JOG_UNKNOWN;
}
