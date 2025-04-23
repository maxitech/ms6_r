#include "Kinematics.h"

Kinematics::Kinematics(std::vector<MotorConfig>& motorConfigs)
    : _motorConfigs(motorConfigs)
{
    for (auto& motorConfig : _motorConfigs)
    {
    }
}

float Kinematics::_totalRatio(const MotorConfig& motorConfig) const
{
    return (motorConfig.gearboxRatio * (motorConfig.drivenTeeth / motorConfig.driverTeeth));
}

float Kinematics::_stepsPerRev(const MotorConfig& motorConfig) const
{
    return (motorConfig.stepsPerRev * motorConfig.microsteps * _totalRatio(motorConfig));
}

float Kinematics::stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const
{
    return (static_cast<float>(currPosInSteps) / _stepsPerRev(motorConfig)) * 360.0f;
}

int Kinematics::degToSteps(const MotorConfig& motorConfig, const float deg) const
{
    return static_cast<int>((deg / 360.0f) * _stepsPerRev(motorConfig));
}

float Kinematics::degToRad(const float deg) const
{
    return deg * (M_PI / 180.0f);
}