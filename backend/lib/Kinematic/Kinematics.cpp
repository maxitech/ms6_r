#include "Kinematics.h"

Kinematics::Kinematics(const std::vector<MotorConfig>& motorConfigs, const std::vector<DHparam>& dhParams)
    : _motorConfigs(motorConfigs)
    , _dhParams(dhParams)
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
    // clang-format off
    m << 
        cos(theta), -sin(theta) * cos(alpha),  sin(theta) * sin(alpha), a * cos(theta),
        sin(theta),  cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta),
        0,             sin(alpha),                  cos(alpha),                d,
        0,             0,                             0,                            1;
    // clang-format on
    return m;
}

Pose Kinematics::forwardKinematics()
{
    Eigen::Matrix4f    T      = Eigen::Matrix4f::Identity();
    std::vector<float> angles = getJointAnglesInRad();

    for (size_t i = 0; i < _dhParams.size(); ++i)
    {
        T *= _dhToTable(_dhParams[i], angles[i]);
    }

    T *= _toolFrameMatrix;

    // Extract the end effector position from the transformation matrix
    const float x = T(0, 3); // x(mm)
    const float y = T(1, 3); // y(mm)
    const float z = T(2, 3); // z(mm)
    // Extract the end effector orientation (roll, pitch, yaw) from the transformation matrix
    const float yaw   = atan2(T(1, 0), T(0, 0));                                      // z(rad)
    const float pitch = atan2(-T(2, 0), sqrt(T(2, 1) * T(2, 1) + T(2, 2) * T(2, 2))); // y(rad)
    const float roll  = atan2(T(2, 1), T(2, 2));

    // x, y, z in mm and roll, pitch, yaw in degrees
    return {x, y, z, _radToDeg(roll), _radToDeg(pitch), _radToDeg(yaw)};
}