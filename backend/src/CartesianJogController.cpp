#include "CartesianJogController.h"

CartesianJogController::CartesianJogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender) {};

void CartesianJogController::jog_cartesian(const std::vector<String>& arguments, JogState& currJogState)
{
    _handle_jog_cartesian(arguments, currJogState);
}

void CartesianJogController::_handle_jog_cartesian(const std::vector<String>& arguments, JogState& currJogState)
{
    // _arguments -> AXIS,MODE,DIR(-1/1),SPEED,DELTA,EVENT -> {X,LIN,1,15.5,5,START}
    const String& axis  = arguments[0];
    const String& mode  = arguments[1];
    const int     dir   = arguments[2].toInt();
    const float   speed = arguments[3].toFloat();
    const float   delta = arguments[4].toFloat();
    const String& event = arguments.back();

    Pose  currPos      = _getCurrPose();
    float intervall_ms = _calcIntervall(delta, speed);
    Pose  newPos       = _calcNextPos(mode, axis, delta, currPos);

    // put new_pos in IK() -> get next joint angles
    // move all joints simultaniously to next pos
    // pay attention to joint limits and futre workspace limits
}

Pose CartesianJogController::_getCurrPose()
{
    return _kin.forwardKinematics();
}

float CartesianJogController::_calcIntervall(const float delta, const float speed)
{
    float intervall_ms = (delta / speed) * 1000.0f;
    intervall_ms       = std::clamp(intervall_ms, 20.0f, 70.0f);
    return intervall_ms;
}

Pose CartesianJogController::_calcNextPos(const String& mode, const String& axis, const float delta, const Pose& currPos)
{
    Pose newPos;
    if (mode == "LIN")
    {
        if (axis == "X")
        {
            newPos.x = currPos.x + static_cast<double>(delta);
        }
        else if (axis == "Y")
        {
            newPos.y = currPos.y + static_cast<double>(delta);
        }
        else if (axis == "Z")
        {
            newPos.z = currPos.z + static_cast<double>(delta);
        }
        else
        {
            Serial.print("Error: Mode LIN has no axis: ");
            Serial.print(axis);
            Serial.println(".");
        }
    }
    else if (mode == "ROT")
    {
        if (axis == "RX")
        {
            newPos.roll = currPos.roll + static_cast<double>(delta);
        }
        else if (axis == "RY")
        {
            newPos.pitch = currPos.pitch + static_cast<double>(delta);
        }
        else if (axis == "RZ")
        {
            newPos.yaw = currPos.yaw + static_cast<double>(delta);
        }
        else
        {
            Serial.print("Error: Mode ROT has no axis: ");
            Serial.print(axis);
            Serial.println(".");
        }
    }
    else
    {
        Serial.print("Error: Unknown Mode: ");
        Serial.print(mode);
        Serial.println(".");
    }
    return newPos;
}