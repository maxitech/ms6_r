#ifndef ROBOTDATASENDER_H
#define ROBOTDATASENDER_H

#include "ComProtocoll.h"
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
    // void sendFkPoseAndJointAngles();

    /**
     * @brief Sends the current motor position in steps to the serial output.
     * @param motorConfig Vector of motor configs.
     * @internal
     */
    void sendMotorPosInSteps(const std::vector<MotorConfig*>& motorConfigs);

    void setHomedMask(const uint8_t homedMask) { _homedMask = homedMask; };

private:
    uint8_t _homedMask;
};

#endif // ROBOTDATASENDER_H