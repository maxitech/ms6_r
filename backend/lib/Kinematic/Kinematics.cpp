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
    m << cos(theta), -sin(theta) * cos(alpha), sin(theta) * sin(alpha), a * cos(theta),
         sin(theta), cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta),
         0, sin(alpha), cos(alpha), d,
         0, 0, 0, 1;
    // clang-format on
    return m;
}

Eigen::Matrix4d Kinematics::_createTransformationMatrix(double x, double y, double z, double yaw, double pitch, double roll) const
{
    Eigen::AngleAxisd rollAngle(_degToRad(roll), Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd pitchAngle(_degToRad(pitch), Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd yawAngle(_degToRad(yaw), Eigen::Vector3d::UnitZ());

    Eigen::Quaterniond q = yawAngle * pitchAngle * rollAngle;
    Eigen::Matrix3d    R = q.toRotationMatrix();

    Eigen::Matrix4d T   = Eigen::Matrix4d::Identity();
    T.block<3, 3>(0, 0) = R;
    T(0, 3)             = x;
    T(1, 3)             = y;
    T(2, 3)             = z;

    // Debug output
    // std::cout << std::fixed << std::setprecision(6);
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

    // Compute full transformation matrix
    for (size_t i = 0; i < _dhParams.size(); ++i)
    {
        T *= _dhToTable(_dhParams[i], angles[i]);
    }

    // Apply tool frame transformation
    T *= _toolFrameMatrix;

    // Extract position (translation part)
    const double x = T(0, 3);
    const double y = T(1, 3);
    const double z = T(2, 3);

    // Extract rotation matrix (upper-left 3x3 submatrix)
    Eigen::Matrix3d R = T.block<3, 3>(0, 0);

    // Convert rotation matrix to quaternion for better stability
    Eigen::Quaterniond q(R);
    Eigen::Matrix3d    RotFromQuat = q.toRotationMatrix();

    // Compute Euler angles (ZYX convention: Yaw-Z, Pitch-Y, Roll-X)
    double pitch, roll, yaw;
    bool   inSingularity = false;

    constexpr double eps = 1e-6; // Precision threshold

    if (std::abs(RotFromQuat(2, 0)) > 1.0 - eps)
    {
        inSingularity = true;

        // Prevent precision errors when near ±1
        pitch = _degToRad(RotFromQuat(2, 0) > 0 ? 90.0 : -90.0);
        roll  = std::atan2(RotFromQuat(2, 1), RotFromQuat(2, 2));
        yaw   = std::atan2(RotFromQuat(1, 0), RotFromQuat(0, 0));
    }
    else
    {
        pitch = std::asin(-RotFromQuat(2, 0));
        roll  = std::atan2(RotFromQuat(2, 1), RotFromQuat(2, 2));
        yaw   = std::atan2(RotFromQuat(1, 0), RotFromQuat(0, 0));
    }

    // Debug output
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "End effector position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
    std::cout << "End effector orientation (deg): roll=" << _radToDeg(roll)
              << ", pitch=" << _radToDeg(pitch) << ", yaw=" << _radToDeg(yaw) << std::endl;
    if (inSingularity)
    {
        std::cout << "[WARNING] Gimbal lock detected – Adjusted angles dynamically.\n";
    }
    std::cout << "Transformation matrix fk:\n"
              << T << std::endl;

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
