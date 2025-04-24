#include "Kinematics.h"

Kinematics::Kinematics(const std::vector<MotorConfig>& motorConfigs, const std::vector<DHparam>& dhParams)
    : _dhParams(dhParams)
    , _motorConfigs(motorConfigs)
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

Eigen::Matrix4f Kinematics::_dhToTable(const DHparam& param, const float theta) const
{
    float alpha = param.alpha;
    float a     = param.a;
    float d     = param.d;

    Eigen::Matrix4f m;
    m << cos(theta), -sin(theta) * cos(alpha), sin(theta) * sin(alpha), a * cos(theta),
        sin(theta), cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta),
        0, sin(alpha), cos(alpha), d,
        0, 0, 0, 1;

    return m;
}