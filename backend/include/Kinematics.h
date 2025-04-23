#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "MotorConfig.h"
#include <Arduino.h>
#include <cmath>

class Kinematics
{
public:
    explicit Kinematics(std::vector<MotorConfig>& motorConfigs);

private:
    float _totalRatio(const MotorConfig& motorConfig) const;
    float _stepsPerRev(const MotorConfig& motorConfig) const;
    float _stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const;
    int   _degToSteps(const MotorConfig& motorConfig, const float angle) const;
    float _degToRad(const float angle) const;

    std::vector<MotorConfig> _motorConfigs;
};

#endif // KINEMATICS_H