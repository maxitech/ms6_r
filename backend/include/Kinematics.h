#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "MotorConfig.h"
#include <Arduino.h>
#include <cmath>

class Kinematics
{
public:
    explicit Kinematics(std::vector<MotorConfig>& motorConfigs);

    float stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const;
    int   degToSteps(const MotorConfig& motorConfig, const float deg) const;
    float degToRad(const float deg) const;

private:
    float _totalRatio(const MotorConfig& motorConfig) const;
    float _stepsPerRev(const MotorConfig& motorConfig) const;

    std::vector<MotorConfig> _motorConfigs;
};

#endif // KINEMATICS_H