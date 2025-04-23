#include "Kinematics.h"

Kinematics::Kinematics(const std::vector<MotorConfig>& motorConfigs)
    : _motorConfigs(motorConfigs)
{
}

float Kinematics::_totalRatio(const MotorConfig& motorConfig) const
{
    return (motorConfig.gearboxRatio * (motorConfig.drivenTeeth / motorConfig.driverTeeth));
}

float Kinematics::_stepsPerRev(const MotorConfig& motorConfig) const
{
    return (motorConfig.stepsPerRev * motorConfig.microsteps * _totalRatio(motorConfig));
}

float Kinematics::_stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const
{
    return (static_cast<float>(currPosInSteps) / _stepsPerRev(motorConfig)) * 360.0f;
}

int Kinematics::_degToSteps(const MotorConfig& motorConfig, const float deg) const
{
    return static_cast<int>((deg / 360.0f) * _stepsPerRev(motorConfig));
}

float Kinematics::_degToRad(const float deg) const
{
    return deg * (M_PI / 180.0f);
}

float Kinematics::_radToDeg(const float rad) const
{
    return rad * (180.0f / M_PI);
}

std::vector<float> Kinematics::getJointAnglesInRad() const
{
    std::vector<float> angles;
    angles.reserve(_motorConfigs.size());

    for (const auto& cfg : _motorConfigs)
    {
        float deg = _stepsToDeg(cfg, cfg.motor->getPosition());
        angles.push_back(_degToRad(deg));
    }

    return angles;
}