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

Eigen::Matrix4d Kinematics::_dhToTable(const DHparam& param, double theta, double offsetDeg) const
{
    double alpha = param.alpha;
    double a     = param.a;
    double d     = param.d;

    double thetaOffset = theta + _degToRad(offsetDeg);

    Eigen::Matrix4d m;
    // clang-format off
    m << cos(thetaOffset), -sin(thetaOffset) * cos(alpha), sin(thetaOffset) * sin(alpha), a * cos(thetaOffset),
         sin(thetaOffset), cos(thetaOffset) * cos(alpha), -cos(thetaOffset) * sin(alpha), a * sin(thetaOffset),
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
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "End effector position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
    // std::cout << "End effector orientation (deg): roll=" << _radToDeg(roll)
    //           << ", pitch=" << _radToDeg(pitch) << ", yaw=" << _radToDeg(yaw) << std::endl;
    // if (inSingularity)
    // {
    //     std::cout << "[WARNING] Gimbal lock detected – Adjusted angles dynamically.\n";
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

    // 3) Calculate J1 angle (next step)
    // calc j1 angle
    double x_r05 = R_0_5(0, 3);
    double y_r05 = R_0_5(1, 3);

    double theta1Rad = std::atan2(y_r05, x_r05); // atan2(y, x)
    double theta1Deg = _radToDeg(theta1Rad);

    // Set J1 angle mathematical zero for further calculations
    // ? use (-theta1Rad) instead of theta1Rad: why? because we want to rotate back to zero
    double x_at_j1_zero = x_r05 * cos(-theta1Rad) - y_r05 * sin(-theta1Rad);
    double y_at_j1_zero = y_r05 * cos(-theta1Rad) + x_r05 * sin(-theta1Rad);

    double L1 = std::abs(x_at_j1_zero - _dhParams[0].a);
    double L4 = R_0_5(2, 3) - _dhParams[0].d;                          // Z value of R_0_5 matrix - d value of J1
    double L2 = sqrt(pow(L1, 2) + pow(L4, 2));                         // L2 = sqrt(L1^2 + L4^2)
    double L3 = sqrt(pow(_dhParams[3].d, 2) + pow(_dhParams[2].a, 2)); // L3 = sqrt(d^2 + a^2)

    // theha b = deg(atan(L1/L4)
    double THETA_B = _radToDeg(std::atan2(L1, L4)); // atan2(L1, L4)

    double THETA_C = _radToDeg(std::acos((pow(_dhParams[1].a, 2) + pow(L2, 2) - pow(L3, 2)) / (2 * _dhParams[1].a * L2)));

    double THETA_D = _radToDeg(std::acos((pow(L3, 2) + pow(_dhParams[1].a, 2) - pow(L2, 2)) / (2 * L3 * _dhParams[1].a)));

    double THETA_E = _radToDeg(std::atan2(_dhParams[2].a, _dhParams[3].d));

    double theta2Deg;
    if (x_at_j1_zero > _dhParams[0].a)
    {
        if (L4 > 0)
        {
            theta2Deg = THETA_B - THETA_C;
        }
        else
        {
            theta2Deg = THETA_B - THETA_C + 180.0f;
        }
    }
    else
    {
        theta2Deg = -(THETA_B + THETA_C);
    }

    double theta3Deg = -(THETA_D + THETA_E) + 90.0f;

    // Use J1, J2, J3 angles to find J3 orientation
    Eigen::Matrix4d DH_J1 = _dhToTable(_dhParams[0], theta1Rad);
    Eigen::Matrix4d DH_J2 = _dhToTable(_dhParams[1], _degToRad(theta2Deg), -90.0f);
    Eigen::Matrix4d DH_J3 = _dhToTable(_dhParams[2], _degToRad(theta3Deg), 180.0f);

    Eigen::Matrix4d R_0_3 = DH_J1 * DH_J2 * DH_J3; // R_0_3 matrix

    // Merge overall rotation(R_0_6) with R_0_3 matrix
    // Transpose R_0_3 matrix and multiply with R_0_6 matrix
    Eigen::Matrix3d R_0_3_T = R_0_3.block<3, 3>(0, 0).transpose(); // Transpose of the upper-left 3x3 submatrix of R_0_3

    // Calculate R_3_6 matrix (R_0_3_T * R_0_6<3,3>) -> actual rotation of spherical wrist
    Eigen::Matrix3d R_3_6 = R_0_3_T * R_0_6.block<3, 3>(0, 0);
    // extract J4, J5, J6 angles from R_3_6 matrix

    // Determine if the wrist is flipped (J5 angle negative)
    bool                flipWrist = false;
    std::vector<double> angles    = getJointAnglesInRad();
    if (angles.size() >= 6) // ? Avoid out-of-bounds access – update this if more joints are added
    {
        flipWrist = angles[4] < 0;
    }

    double theta5Rad = std::acos(R_3_6(2, 2));
    double theta4Rad, theta6Rad;

    if (std::abs(theta5Rad) < 1e-6)
    {
        // Singularity: theta5 is ~0 → wrist is aligned → set default values
        theta4Rad = 0;
        theta6Rad = std::atan2(-R_3_6(0, 1), R_3_6(0, 0));
    }
    else
    {
        // Optional: flipWrist = true → Inverse ik-solution w. J5 negative
        if (!flipWrist)
        {
            theta4Rad = std::atan2(R_3_6(1, 2), R_3_6(0, 2));
            theta6Rad = std::atan2(R_3_6(2, 1), -R_3_6(2, 0));
            // std::cout << "[INFO] Wrist not flipped – J5 positive.\n";
        }
        else
        {
            // Flip wrist: invert theta5 and negate directions for theta4/theta6
            theta5Rad *= -1;
            theta4Rad = std::atan2(-R_3_6(1, 2), -R_3_6(0, 2));
            theta6Rad = std::atan2(-R_3_6(2, 1), R_3_6(2, 0));
            // std::cout << "[INFO] Wrist flipped – J5 negative.\n";
        }
    }

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
    // -----------------------Geometric values----------------------
    // std::cout << "x_at_j1_zero: " << x_at_j1_zero << std::endl;
    // std::cout << "y_at_j1_zero: " << y_at_j1_zero << std::endl;
    // std::cout << "L1: " << L1 << std::endl;
    // std::cout << "L2: " << L2 << std::endl;
    // std::cout << "L3: " << L3 << std::endl;
    // std::cout << "L4: " << L4 << std::endl;
    // std::cout << "_dhParams[0].a: " << _dhParams[0].a << std::endl;
    // std::cout << "_dhParams[0].d: " << _dhParams[0].d << std::endl;
    // std::cout << "Theta B (deg): " << THETA_B << std::endl;
    // std::cout << "Theta C (deg): " << THETA_C << std::endl;
    // std::cout << "Theta D (deg): " << THETA_D << std::endl;
    // std::cout << "Theta E (deg): " << THETA_E << std::endl;
    // ------------------------Angles result------------------------
    // std::cout << "Theta1 (deg): " << theta1Deg << std::endl;
    // std::cout << "Theta2 (deg): " << theta2Deg << std::endl;
    // std::cout << "Theta3 (deg): " << theta3Deg << std::endl;
    // std::cout << "Theta4 (deg): " << _radToDeg(theta4Rad) << std::endl;
    // std::cout << "Theta5 (deg): " << _radToDeg(theta5Rad) << std::endl;
    // std::cout << "Theta6 (deg): " << _radToDeg(theta6Rad) << std::endl;
    // std::cout << "DH_J1\n"
    //           << DH_J1 << std::endl;
    // std::cout << "DH_J2\n"
    //           << DH_J2 << std::endl;
    // std::cout << "DH_J3\n"
    //           << DH_J3 << std::endl;
    // std::cout << "R_0_3:\n"
    //           << R_0_3 << std::endl;
    // std::cout << "R_0_3_T:\n"
    //           << R_0_3_T << std::endl;
    // std::cout << "R_3_6:\n"
    //           << R_3_6 << std::endl;

    return {theta1Deg, theta2Deg, theta3Deg, _radToDeg(theta4Rad), _radToDeg(theta5Rad), _radToDeg(theta6Rad)};
}