#include "Kinematics.h"

Kinematics::Kinematics(const std::vector<MotorConfig>& motorConfigs, const std::vector<DHparam>& dhParams)
    : _motorConfigs(motorConfigs)
    , _dhParams(dhParams)
{
}

double Kinematics::_totalRatio(const MotorConfig& motorConfig) const
{
    return (motorConfig.gearboxRatio * (motorConfig.drivenTeeth / motorConfig.driverTeeth));
}

double Kinematics::_stepsPerRev(const MotorConfig& motorConfig) const
{
    return (motorConfig.stepsPerRev * motorConfig.microsteps * _totalRatio(motorConfig));
}

double Kinematics::_stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const
{
    return (static_cast<double>(currPosInSteps) / _stepsPerRev(motorConfig)) * 360.0;
}

int Kinematics::_degToSteps(const MotorConfig& motorConfig, const double deg) const
{
    return static_cast<int>((deg / 360.0) * _stepsPerRev(motorConfig));
}

double Kinematics::_degToRad(const double deg) const
{
    return deg * (M_PI / 180.0);
}

double Kinematics::_radToDeg(const double rad) const
{
    return rad * (180.0 / M_PI);
}

Eigen::Matrix4d Kinematics::_dhToTable(const DHparam& param, const double theta) const
{
    double alpha = param.alpha;
    double a     = param.a;
    double d     = param.d;

    Eigen::Matrix4d m;
    // clang-format off
    m << 
        cos(theta), -sin(theta) * cos(alpha),  sin(theta) * sin(alpha), a * cos(theta),
        sin(theta),  cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta),
        0,             sin(alpha),                  cos(alpha),                d,
        0,             0,                             0,                            1;
    // clang-format on
    return m;
}

Eigen::Matrix4d Kinematics::_createTransformationMatrix(double x, double y, double z, double yaw, double pitch, double roll) const
{
    // Convert angle from deg to rad
    double yawRad   = _degToRad(yaw);
    double pitchRad = _degToRad(pitch);
    double rollRad  = _degToRad(roll);

    // Rotation matrix from ZYX Euler angles (yaw-Z, pitch-Y, roll-X)
    double cy = std::cos(yawRad), sy = std::sin(yawRad);
    double cp = std::cos(pitchRad), sp = std::sin(pitchRad);
    double cr = std::cos(rollRad), sr = std::sin(rollRad);

    // clang-format off
    Eigen::Matrix3d R;
    R << cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr,
         sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr,
        -sp, cp * sr, cp * cr;
    // clang-format on

    // Construct 4x4 homogeneous transformation matrix
    Eigen::Matrix4d T   = Eigen::Matrix4d::Identity();
    T.block<3, 3>(0, 0) = R; // Set rotation part
    // Set translation x, y, z
    T(0, 3) = x;
    T(1, 3) = y;
    T(2, 3) = z;

    // Debug output
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "Position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
    // std::cout << "Orientation (deg): yaw=" << yaw << ", pitch=" << pitch << ", roll=" << roll << std::endl;
    // std::cout << "Transformation Matrix:\n"
    //           << T << std::endl;

    return T;
}

std::vector<double> Kinematics::getJointAnglesInRad() const
{
    std::vector<double> angles;
    if (_motorConfigs.empty() || _motorConfigs.size() > 100)
    {
        return {};
    }

    angles.reserve(_motorConfigs.size());

    for (const auto& cfg : _motorConfigs)
    {
        double deg = _stepsToDeg(cfg, cfg.motor->getPosition());
        angles.push_back(_degToRad(deg));
    }

    return angles;
}

void Kinematics::setToolFrame(double x, double y, double z, double yaw, double pitch, double roll)
{
    _toolFrameMatrix = _createTransformationMatrix(x, y, z, yaw, pitch, roll);
}

Pose Kinematics::forwardKinematics()
{
    Eigen::Matrix4d     T      = Eigen::Matrix4d::Identity();
    std::vector<double> angles = getJointAnglesInRad();

    for (size_t i = 0; i < _dhParams.size(); ++i)
    {
        T *= _dhToTable(_dhParams[i], angles[i]);
    }

    T *= _toolFrameMatrix;

    const double x = T(0, 3);
    const double y = T(1, 3);
    const double z = T(2, 3);

    Eigen::Matrix3d  R   = T.block<3, 3>(0, 0);
    constexpr double eps = 1e-9;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            if (std::abs(R(r, c)) < eps)
                R(r, c) = 0.0;
        }
    }

    double pitch, roll, yaw;
    bool   inSingularity = false;

    if (std::abs(R(2, 0)) < 1.0 - eps)
    {
        pitch = std::asin(-R(2, 0));
        roll  = std::atan2(R(2, 1), R(2, 2));
        yaw   = std::atan2(R(1, 0), R(0, 0));
    }
    else
    {
        inSingularity = true;

        pitch = _degToRad(90.0); // Explicitly force pitch to 90 degrees as double
        roll  = _degToRad(45.0); // Override roll directly in degrees
        yaw   = _degToRad(45.0); // Override yaw directly in degrees
    }

    // Debug output
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "End effector position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
    // std::cout << "End effector orientation (deg): roll=" << _radToDeg(roll)
    //           << ", pitch=" << _radToDeg(pitch) << ", yaw=" << _radToDeg(yaw) << std::endl;
    // if (inSingularity)
    // {
    //     std::cout << "[WARNING] Gimbal lock detected – Roll and Yaw manually overridden.\n";
    // }
    // std::cout << "Transformation matrix fk:\n"
    //           << T << std::endl;

    return {x, y, z, _radToDeg(roll), _radToDeg(pitch), _radToDeg(yaw), inSingularity};
}

Angles Kinematics::inverseKinematics(double x, double y, double z, double yaw, double pitch, double roll)
{
    // Calculate wrist center - find J1 angle
    // 1) From the tip of the end effector to the flange of J6
    // Create transformation matrix from inputs
    Eigen::Matrix4d R_0_6_T = _createTransformationMatrix(x, y, z, yaw, pitch, roll);

    // Tool frame matrix
    // Invert tool frame matrix (multiply tool frame translation with rotation of inverted tool frame,
    // all results have to be negated)
    Eigen::Matrix4d I_T_Toolframe = _toolFrameMatrix.inverse();

    // R_0_6 matrix (flange, end of J6) = Multiply first matrix with the inverted tool frame matrix
    Eigen::Matrix4d R_0_6 = R_0_6_T * I_T_Toolframe;

    // 2) From flange of J6 to spherical wrist center
    // Create identity rotation matrix and replace Z value with the negated DH J6 'd' value
    Eigen::Matrix4d R_0_6_Neg = Eigen::Matrix4d::Identity();
    R_0_6_Neg(2, 3)           = -_dhParams[5].d;

    // Center spherical wrist -> Multiply R_0_6 matrix with the identity rotation matrix
    // one step before = R_0_5
    Eigen::Matrix4d R_0_5 = R_0_6 * R_0_6_Neg;

    // Debug output
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "R_0_6_T:\n"
    //           << R_0_6_T << std::endl;
    // std::cout << "I_T_Toolframe:\n"
    //           << I_T_Toolframe << std::endl;
    // std::cout << "R_0_6:\n"
    //           << R_0_6 << std::endl;
    // std::cout << "R_0_6_Neg:\n"
    //           << R_0_6_Neg << std::endl;
    // std::cout << "R_0_5:\n"
    //           << R_0_5 << std::endl;

    // 3) Calculate J1 angle (next step)

    return {};
}
