#include "CartesianJogController.h"
#include "teensystep4.h"
using namespace TS4;

CartesianJogController::CartesianJogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender) {};

void CartesianJogController::jog_cartesian(const std::vector<String>& arguments, JogState& currJogState)
{
    _handle_jog_cartesian(arguments, currJogState);
}

void CartesianJogController::_handle_jog_cartesian(const std::vector<String>& arguments, JogState& currJogState)
{
    // ***Extracting***
    // _arguments -> AXIS,MODE,DIR(-1/1),SPEED,DELTA,EVENT -> {X,LIN,1,15.5,5,START}
    const int            dir   = arguments[2].toInt();
    static const String& event = arguments.back();

    // for (const String& s : arguments)
    // {
    //     Serial.println(s);
    //     delay(500);
    // }
    // Serial.println(delta);

    // JogFlags&  jogFlags = _jogFlags[motorIdx];
    JogCommand jogCmd = _getJogCommand(event);

    String direction;
    if (dir == 1)
    {
        direction = "POS";
    }
    else if (dir == -1)
    {
        direction = "NEG";
    }

    if (currJogState == JOGGING)
    {
        static unsigned long lastSendTime = 0;
        if (Utils::nonBlockingDelay(100, lastSendTime))
        {
            // Send motor position in steps
            // _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[motorIdx], motorIdx);
            // Send forward kinematics pose and joint angles
            _rbtDtaSender.sendFkPoseAndJointAngles();
        }
    }

    switch (jogCmd)
    {
    case JOG_START:
    {

        // if (jogFlags.limitReached && direction == jogFlags.blockedDir && !jogFlags.runOnce)
        // {
        //     Serial.println("Cannot jog in " + direction + " direction - limit reached");
        //     jogFlags.runOnce = true;
        //     return;
        // }

        // if (jogFlags.limitReached && direction != jogFlags.blockedDir)
        // {
        //     jogFlags.limitReached = false;
        //     jogFlags.runOnce      = false;
        //     jogFlags.blockedDir   = "";
        // }

        // if (currJogState != JOGGING && !jogFlags.limitReached)

        // const String direction = dir == 1 ? "POS" : "NEG";

        if (currJogState != JOGGING)
        {
            // start
            Serial.println("START");
            currJogState = JOGGING;
            _isJogging   = true;

            Pose currPos = _getCurrPose();
            if (currPos.inSingularity)
            {
                currJogState = IDLE_JOG;
                Serial.println("[FK] Warning: In singularity - move not allowed. Please Jog out of this position via joint mode.");
                return;
            }
            _lockedOrientation.roll  = currPos.roll;
            _lockedOrientation.pitch = currPos.pitch;
            _lockedOrientation.yaw   = currPos.yaw;

            // Initiale Variante setzen
            Angles currAngles = _kin->inverseKinematics(currPos.x, currPos.y, currPos.z,
                                                        currPos.yaw, currPos.pitch, currPos.roll,
                                                        true);

            _kin->setCurrentVariant(currAngles.shoulderVariant,
                                    currAngles.elbowVariant,
                                    currAngles.wristFlipped);

            Serial.print("T1: ");
            Serial.println(currAngles.theta1);
            Serial.print("T2: ");
            Serial.println(currAngles.theta2);
            Serial.print("T3: ");
            Serial.println(currAngles.theta3);
            Serial.print("T4: ");
            Serial.println(currAngles.theta4);
            Serial.print("T5: ");
            Serial.println(currAngles.theta5);
            Serial.print("T6: ");
            Serial.println(currAngles.theta6);

            if (direction != "POS" && direction != "NEG")
            {
                Serial.println("Invalid direction string: " + direction);
                return;
            }
        }

        _moveToPos(arguments);

        // else if (currJogState == JOGGING && !jogFlags.limitReached)
        // {
        //     std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);
        //     float               currPosDeg  = jointAngles[motorIdx];
        //     if (currPosDeg >= _motorConfigs[motorIdx]->minAngleDeg ||
        //         currPosDeg <= _motorConfigs[motorIdx]->maxAngleDeg)
        //     {
        //         _motorConfigs[motorIdx]->motor->emergencyStop();
        //         currJogState          = IDLE_JOG;
        //         jogFlags.limitReached = true;
        //         jogFlags.blockedDir   = direction;

        //         _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[motorIdx], motorIdx);
        //         _rbtDtaSender.sendFkPoseAndJointAngles();
        //     }
        // }
        break;
    }

    case JOG_STOP:
    {

        // if (currJogState == JOGGING && !jogFlags.limitReached)
        if (currJogState == JOGGING)
        {
            // stop
            for (MotorConfig* cfg : _motorConfigs)
            {
                cfg->motor->emergencyStop();
            }
            currJogState = IDLE_JOG;
            _isJogging   = true;
            Serial.println("Stopped");
            // _rbtDtaSender.sendMotorPosInSteps(_motorConfigs[motorIdx], motorIdx);
            // _rbtDtaSender.sendFkPoseAndJointAngles();
        }
        break;
    }

    case JOG_UNKNOWN:
    default:
    {
        Serial.println("Unknown JOG state: " + event);
        break;
    }
    }
}

JogCommand CartesianJogController::_getJogCommand(const String& str)
{
    if (str == "START")
        return JOG_START;
    else if (str == "STOP")
        return JOG_STOP;
    else
        return JOG_UNKNOWN;
}

Pose CartesianJogController::_getCurrPose()
{
    return _kin->forwardKinematics();
}

float CartesianJogController::_calcIntervall(const float delta, const float speed)
{
    float intervall_ms = (delta / speed) * 1000.0f;
    intervall_ms       = std::clamp(intervall_ms, 20.0f, 70.0f);
    return intervall_ms;
}

Pose CartesianJogController::_calcNextPos(const String& mode, const String& axis, const float delta, const int dir, const Pose& currPos)
{
    Pose newPos = currPos;
    if (mode == "LIN")
    {
        if (axis == "X")
        {
            newPos.x += static_cast<double>(delta) * dir;
        }
        else if (axis == "Y")
        {
            newPos.y += static_cast<double>(delta) * dir;
        }
        else if (axis == "Z")
        {
            newPos.z += static_cast<double>(delta) * dir;
        }
        else
        {
            Serial.print("Error: Mode LIN has no axis: ");
            Serial.print(axis);
            Serial.println(".");
        }
        newPos.roll  = _lockedOrientation.roll;
        newPos.pitch = _lockedOrientation.pitch;
        newPos.yaw   = _lockedOrientation.yaw;
    }
    else if (mode == "ROT")
    {
        if (axis == "RX")
        {
            newPos.roll += static_cast<double>(delta) * dir;
        }
        else if (axis == "RY")
        {
            newPos.pitch += static_cast<double>(delta) * dir;
        }
        else if (axis == "RZ")
        {
            newPos.yaw += static_cast<double>(delta) * dir;
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

Angles CartesianJogController::_calcNextPosAngles(const Pose& nextPos)
{
    return _kin->inverseKinematics(nextPos.x, nextPos.y, nextPos.z, nextPos.yaw, nextPos.pitch, nextPos.roll, _isJogging);
}

void CartesianJogController::_moveToPos(const std::vector<String>& arguments)
{
    static StepperGroup group;
    static bool         isInit = false;
    // Serial.print("T1: ");
    // Serial.println(angles.theta1);
    // Serial.print("T2: ");
    // Serial.println(angles.theta2);
    // Serial.print("T3: ");
    // Serial.println(angles.theta3);
    // Serial.print("T4: ");
    // Serial.println(angles.theta4);
    // Serial.print("T5: ");
    // Serial.println(angles.theta5);
    // Serial.print("T6: ");
    // Serial.println(angles.theta6);

    // limit check

    if (!isInit)
    {
        group.clear();
        for (MotorConfig* cfg : _motorConfigs)
        {
            group.add(cfg->motor);
        }
        isInit = true;
    }

    if (group.gIsFinished())
    {
        // ***Extracting***
        // _arguments -> AXIS,MODE,DIR(-1/1),SPEED,DELTA,EVENT -> {X,LIN,1,15.5,5,START}
        const String& axis  = arguments[0];
        const String& mode  = arguments[1];
        const int     dir   = arguments[2].toInt();
        const float   speed = arguments[3].toFloat();
        const float   delta = arguments[4].toFloat();

        // ***Calculations***
        Pose currPos = _getCurrPose();
        if (currPos.inSingularity)
        {
            Serial.println("[FK] Warning: In singularity - move not allowed. Please Jog out of this position via joint mode.");
            return;
        }
        // float  intervall_ms  = _calcIntervall(delta, speed);
        Pose nextPos = _calcNextPos(mode, axis, delta, dir, currPos);
        // Serial.println(nextPos.inSingularity);
        // Serial.println(nextPos.z);
        // Serial.println(nextPos.yaw);
        // Serial.println(nextPos.pitch);
        // Serial.println(nextPos.roll);
        Angles nextPosAngles = _calcNextPosAngles(nextPos);

        if (nextPosAngles.theta1 == -1)
        {
            // Serial.println("No valid solution.");
            return;
        }

        // _moveToPos(nextPosAngles);
        // move all joints simultaniously to next pos
        // pay attention to joint limits and futre workspace limits

        std::vector<int> steps = _convertAngToSteps(nextPosAngles);
        // std::vector<int> steps = {0, 1296, -1041, 45, 592, 6388};

        // for (int& i : steps)
        // {
        //     Serial.print("Steps M:");
        //     Serial.println(i);
        // }

        size_t i = 0;
        for (Stepper* s : group)
        {
            s->setTargetAbs(steps[i++]);
        }
        group.startMove();
    }
}

std::vector<int> CartesianJogController::_convertAngToSteps(const Angles& angles)
{
    std::array<double, 6> allAngles = {
        angles.theta1, angles.theta2, angles.theta3,
        angles.theta4, angles.theta5, angles.theta6};

    // for (double& angle : allAngles)
    // {
    //     Serial.println(angle);
    // }

    std::vector<int> steps;
    for (size_t i = 0; i < allAngles.size(); ++i)
    {
        int stepCount = _kin->degToSteps(_motorConfigs[i], allAngles[i]) + _motorConfigs[i]->homeOffsetSteps;
        steps.push_back(stepCount);
    }
    return steps;
}