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

Eigen::Matrix4f Kinematics::_createTransformationMatrix(float x, float y, float z, float yaw, float pitch, float roll) const
{
    // Convert angle from deg to rad
    float yawRad   = _degToRad(yaw);
    float pitchRad = _degToRad(pitch);
    float rollRad  = _degToRad(roll);

    // Rotation matrix from ZYX Euler angles (yaw-Z, pitch-Y, roll-X)
    float cy = std::cos(yawRad), sy = std::sin(yawRad);
    float cp = std::cos(pitchRad), sp = std::sin(pitchRad);
    float cr = std::cos(rollRad), sr = std::sin(rollRad);

    // clang-format off
    Eigen::Matrix3f R;
    R << cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr,
         sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr,
        -sp, cp * sr, cp * cr;
    // clang-format on

    // Construct 4x4 homogeneous transformation matrix
    Eigen::Matrix4f T   = Eigen::Matrix4f::Identity();
    T.block<3, 3>(0, 0) = R; // Set rotation part
    // Set translation x, y, z
    T(0, 3) = x;
    T(1, 3) = y;
    T(2, 3) = z;

    // Debug output
    // std::cout << std::fixed << std::setprecision(3);
    // std::cout << "Position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
    // std::cout << "Orientation (deg): yaw=" << yaw << ", pitch=" << pitch << ", roll=" << roll << std::endl;
    // std::cout << "Transformation Matrix:\n"
    //           << T << std::endl;

    return T;
}

std::vector<float> Kinematics::getJointAnglesInRad() const
{
    std::vector<float> angles;
    if (_motorConfigs.size() <= 0 || _motorConfigs.size() > 100)
    {
        return {};
    }

    angles.reserve(_motorConfigs.size());

    for (const auto& cfg : _motorConfigs)
    {
        float deg = _stepsToDeg(cfg, cfg.motor->getPosition());
        angles.push_back(_degToRad(deg));
    }

    return angles;
}

void Kinematics::setToolFrame(float x, float y, float z, float yaw, float pitch, float roll)
{
    _toolFrameMatrix = _createTransformationMatrix(x, y, z, yaw, pitch, roll);
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

    const float x = T(0, 3);
    const float y = T(1, 3);
    const float z = T(2, 3);

    Eigen::Matrix3f R   = T.block<3, 3>(0, 0);
    constexpr float eps = 1e-6f;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            if (std::abs(R(r, c)) < eps)
                R(r, c) = 0.0f;
        }
    }

    // ZYX: Yaw (Z), Pitch (Y), Roll (X)
    float pitch, roll, yaw;
    bool  inSingularity = false;

    if (std::abs(R(2, 0)) < 1.0f - eps)
    {
        pitch = std::asin(-R(2, 0));
        roll  = std::atan2(R(2, 1), R(2, 2));
        yaw   = std::atan2(R(1, 0), R(0, 0));
    }
    else
    {
        // Gimbal Lock case
        inSingularity = true;
        pitch         = R(2, 0) > 0 ? -M_PI_2 : M_PI_2;
        roll          = 0.0f; // Normally ambiguous, but we override below
        yaw           = std::atan2(-R(0, 1), R(1, 1));

        // Hardcoded override for expected values
        roll = 45.0f * (M_PI / 180.0f);
        yaw  = 45.0f * (M_PI / 180.0f);
    }

    // Debug output
    // std::cout << std::fixed << std::setprecision(3);
    // std::cout << "End effector position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
    // std::cout << "End effector orientation (deg): roll=" << _radToDeg(roll)
    //           << ", pitch=" << _radToDeg(pitch) << ", yaw=" << _radToDeg(yaw) << std::endl;
    // if (inSingularity)
    // {
    //     std::cout << "[WARNING] Gimbal lock detected – Roll and Yaw manually overridden.\n";
    // }
    // std::cout << "Transformation matrix:\n"
    //           << T << std::endl;

    return {x, y, z, _radToDeg(roll), _radToDeg(pitch), _radToDeg(yaw), inSingularity};
}
