#ifndef ROBOTDATASENDER_H
#define ROBOTDATASENDER_H

#include "MotorConfig.h"
#include <Arduino.h>

class RobotDataSender
{
public:
    RobotDataSender();

    /**
     * @brief Sends the forward kinematics pose and joint angles to the serial output.
     * @internal
     */
    void sendFkPoseAndJointAngles();

    /**
     * @brief Sends the current motor position in steps to the serial output.
     * @param motorIdx The index of the motor to send the position for.
     * @internal
     */
    void sendMotorPosInSteps(const MotorConfig* motorConfig, const int motorIdx);

private:
};

#endif // ROBOTDATASENDER_H