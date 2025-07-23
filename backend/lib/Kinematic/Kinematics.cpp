#include "Kinematics.h"
#include "iomanip"
#include "iostream"
#include <algorithm>

Kinematics::Kinematics(const std::vector<MotorConfig*>& motorConfigs, const std::array<DHparam, Utils::NUM_DOF>& dhParams)
    : _motorConfigs(motorConfigs)
    , _dhParams(dhParams)
{
}

static inline double _clamp01(double v)
{
    return (v > 1.0 ? 1.0 : (v < -1.0 ? -1.0 : v));
}

bool Kinematics::_isValidConfig(const MotorConfig* cfg) const
{
    return cfg != nullptr && cfg->motor != nullptr && cfg->stepsPerRev > 0 && cfg->microsteps > 0;
}

double Kinematics::_totalRatio(const MotorConfig* motorConfig) const
{
    if (!_isValidConfig(motorConfig))
    {
        return 0.0;
    }
    return (motorConfig->gearboxRatio * (motorConfig->drivenTeeth / motorConfig->driverTeeth));
}

double Kinematics::_stepsPerRev(const MotorConfig* motorConfig) const
{
    if (!_isValidConfig(motorConfig))
    {
        return 0.0;
    }
    return (motorConfig->stepsPerRev * motorConfig->microsteps * _totalRatio(motorConfig));
}

double Kinematics::_stepsToDeg(const MotorConfig* motorConfig, const int currPosInSteps) const
{
    if (!_isValidConfig(motorConfig))
    {
        return 0.0;
    }
    int relativeSteps = currPosInSteps - motorConfig->homeOffsetSteps;
    return (static_cast<double>(relativeSteps) / _stepsPerRev(motorConfig)) * 360.0;
}

int Kinematics::degToSteps(const MotorConfig* motorConfig, const double deg) const
{
    if (!_isValidConfig(motorConfig))
    {
        return 0;
    }
    return static_cast<int>(std::lround((deg / 360.0) * _stepsPerRev(motorConfig)));
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

std::vector<double> Kinematics::getJointAnglesInRadOrDeg(const int radOrDeg) const
{

    if (_motorConfigs.empty() || _motorConfigs.size() != Utils::NUM_DOF) // ensure not more than 6 motors
    {
        return {};
    }
    std::vector<double> anglesRad;
    std::vector<double> anglesDeg;

    anglesRad.reserve(Utils::NUM_DOF);
    anglesDeg.reserve(Utils::NUM_DOF);

    for (const auto& cfg : _motorConfigs)
    {
        double deg = _stepsToDeg(cfg, cfg->motor->getPosition());
        anglesDeg.push_back(deg);
        anglesRad.push_back(_degToRad(deg));
    }
    if (radOrDeg == 1) // 1 for radians, 0 for degrees
    {
        return anglesRad; // already in radians
    }
    else if (radOrDeg == 0)
    {
        return anglesDeg; // return degrees
    }
    else
    {
        return {};
    }
}

void Kinematics::setToolFrame(double x, double y, double z, double yaw, double pitch, double roll)
{
    _toolFrameMatrix = _createTransformationMatrix(x, y, z, yaw, pitch, roll);
}

// Pose Kinematics::forwardKinematics()
// {
//     Eigen::Matrix4d     T      = Eigen::Matrix4d::Identity();
//     std::vector<double> angles = getJointAnglesInRadOrDeg(1); // Get angles in radians
//     angles[2]                  = -angles[2];                  // Invert angle for J3 to get correct Z Position.

//     // Compute full transformation matrix
//     for (size_t i = 0; i < _dhParams.size(); ++i)
//     {
//         T *= _dhToTable(_dhParams[i], angles[i], _dhParams[i].thetaOffset);
//     }

//     // Apply tool frame transformation
//     T *= _toolFrameMatrix;

//     // Extract position (translation part)
//     const double x = T(0, 3);
//     const double y = T(1, 3);
//     const double z = T(2, 3);

//     // Extract rotation matrix (upper-left 3x3 submatrix)
//     Eigen::Matrix3d R = T.block<3, 3>(0, 0);

//     // Convert rotation matrix to quaternion for better stability
//     Eigen::Quaterniond q(R);
//     Eigen::Matrix3d    RotFromQuat = q.toRotationMatrix();

//     // Compute Euler angles (ZYX convention: Yaw-Z, Pitch-Y, Roll-X)
//     double pitch, roll, yaw;
//     bool   inSingularity = false;

//     constexpr double eps = 1e-6; // Precision threshold

//     // if (std::abs(std::abs(_radToDeg(pitch)) - 90.0) < 0.5) // 0.5°
//     if (std::abs(RotFromQuat(2, 0)) > 1.0 - eps)
//     {
//         inSingularity = true;

//         // Prevent precision errors when near ±1
//         pitch = _degToRad(RotFromQuat(2, 0) > 0 ? 90.0 : -90.0);
//         roll  = std::atan2(RotFromQuat(2, 1), RotFromQuat(2, 2));
//         yaw   = std::atan2(RotFromQuat(1, 0), RotFromQuat(0, 0));
//     }
//     else
//     {
//         pitch = std::asin(-RotFromQuat(2, 0));
//         roll  = std::atan2(RotFromQuat(2, 1), RotFromQuat(2, 2));
//         yaw   = std::atan2(RotFromQuat(1, 0), RotFromQuat(0, 0));
//     }

//     // Debug output
//     // std::cout << std::fixed << std::setprecision(6);
//     // std::cout << "End effector position (mm): x=" << x << ", y=" << y << ", z=" << z << std::endl;
//     // std::cout << "End effector orientation (deg): roll=" << _radToDeg(roll)
//     //           << ", pitch=" << _radToDeg(pitch) << ", yaw=" << _radToDeg(yaw) << std::endl;
//     // if (inSingularity)
//     // {
//     //     std::cout << "[WARNING] Gimbal lock detected – Adjusted angles dynamically.\n";
//     // }
//     // std::cout << "Transformation matrix fk:\n"
//     //           << T << std::endl;

//     return {x, y, z, _radToDeg(roll), _radToDeg(pitch), _radToDeg(yaw), inSingularity};
// }

// Pose Kinematics::forwardKinematics()
// {
//     Eigen::Matrix4d     T      = Eigen::Matrix4d::Identity();
//     std::vector<double> angles = getJointAnglesInRadOrDeg(true); // Radians

//     // Invertierung von J3 nur, wenn mechanisch nötig!
//     // angles[2] = -angles[2]; // Optional

//     // Transformation berechnen über DH-Parameter
//     for (size_t i = 0; i < _dhParams.size(); ++i)
//     {
//         T *= _dhToTable(_dhParams[i], angles[i], _dhParams[i].thetaOffset);
//     }

//     T *= _toolFrameMatrix; // Tool-Versatz reinbringen

//     // Extrahiere Translation
//     const double x = T(0, 3);
//     const double y = T(1, 3);
//     const double z = T(2, 3);

//     // Extrahiere Rotation direkt aus Matrix (ohne Quaternion)
//     Eigen::Matrix3d R = T.block<3, 3>(0, 0);

//     // Berechne Euler-Winkel (ZYX-Konvention: Roll-X, Pitch-Y, Yaw-Z)
//     double roll  = std::atan2(R(2, 1), R(2, 2));
//     double pitch = std::asin(-R(2, 0));
//     double yaw   = std::atan2(R(1, 0), R(0, 0));

//     // Optional: Singularität erkennen, wenn Pitch nahe ±90°
//     bool inSingularity = std::abs(std::abs(_radToDeg(pitch)) - 90.0) < 0.5;

//     return {x, y, z, _radToDeg(roll), _radToDeg(pitch), _radToDeg(yaw), inSingularity};
// }

Pose Kinematics::forwardKinematics()
{
    std::vector<double> anglesRad = getJointAnglesInRadOrDeg(true);
    Eigen::Matrix4d     T         = computeForwardKinematicsMatrix(anglesRad); // T_0_6 inkl. ToolFrame

    Eigen::Vector3d p = T.block<3, 1>(0, 3); // Position
    Eigen::Matrix3d R = T.block<3, 3>(0, 0); // Rotation

    double roll  = std::atan2(R(2, 1), R(2, 2));
    double pitch = std::asin(-R(2, 0));
    double yaw   = std::atan2(R(1, 0), R(0, 0));

    // Singularity check
    Eigen::MatrixXd                   J = computeNumericalJacobian(anglesRad);
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(J);
    double                            maxSigma      = svd.singularValues().maxCoeff();
    double                            minSigma      = svd.singularValues().minCoeff();
    double                            condition     = maxSigma / minSigma;
    bool                              inSingularity = (condition > 1e4);

    return {
        p.x(), p.y(), p.z(),
        _radToDeg(yaw),
        _radToDeg(pitch),
        _radToDeg(roll),
        inSingularity};
}

Eigen::Matrix4d Kinematics::computeForwardKinematicsMatrix(const std::vector<double>& anglesRad)
{
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();

    for (size_t i = 0; i < _dhParams.size(); ++i)
        T *= _dhToTable(_dhParams[i], anglesRad[i], _dhParams[i].thetaOffset);

    T *= _toolFrameMatrix;
    return T;
}

Eigen::MatrixXd Kinematics::computeNumericalJacobian(const std::vector<double>& anglesRad)
{
    const size_t    DOF = anglesRad.size();
    Eigen::MatrixXd J(6, DOF); // 6D: [vx,vy,vz, wx,wy,wz]
    double          eps = 1e-4;

    Eigen::Matrix4d T0 = computeForwardKinematicsMatrix(anglesRad);
    Eigen::Vector3d p0 = T0.block<3, 1>(0, 3);
    Eigen::Matrix3d R0 = T0.block<3, 3>(0, 0);

    for (size_t i = 0; i < DOF; ++i)
    {
        auto perturbed = anglesRad;
        perturbed[i] += eps;

        Eigen::Matrix4d T1 = computeForwardKinematicsMatrix(perturbed);
        Eigen::Vector3d p1 = T1.block<3, 1>(0, 3);
        Eigen::Matrix3d R1 = T1.block<3, 3>(0, 0);

        Eigen::Vector3d   v      = (p1 - p0) / eps;     // Linear
        Eigen::Matrix3d   Rdelta = R1 * R0.transpose(); // Rotationdiff
        Eigen::AngleAxisd aa(Rdelta);
        Eigen::Vector3d   w = aa.axis() * aa.angle() / eps; // Angular

        J.block<3, 1>(0, i) = v;
        J.block<3, 1>(3, i) = w;
    }

    return J;
}

// Angles Kinematics::inverseKinematics(double x, double y, double z, double yaw, double pitch, double roll)
// {
//     // Calculate wrist center - find J1 angle
//     // 1) From the tip of the end effector to the flange of J6
//     // Create transformation matrix from inputs
//     Eigen::Matrix4d R_0_6_T = _createTransformationMatrix(x, y, z, yaw, pitch, roll);

//     // Tool frame matrix
//     // Invert tool frame matrix (multiply tool frame translation with rotation of inverted tool frame,
//     // all results have to be negated)
//     Eigen::Matrix4d I_T_Toolframe = _toolFrameMatrix.inverse();

//     // R_0_6 matrix (flange, end of J6) = Multiply first matrix with the inverted tool frame matrix
//     Eigen::Matrix4d R_0_6 = R_0_6_T * I_T_Toolframe;

//     // 2) From flange of J6 to spherical wrist center
//     // Create identity rotation matrix and replace Z value with the negated DH J6 'd' value
//     Eigen::Matrix4d R_0_6_Neg = Eigen::Matrix4d::Identity();
//     R_0_6_Neg(2, 3)           = -_dhParams[5].d;

//     // Center spherical wrist -> Multiply R_0_6 matrix with the identity rotation matrix
//     // one step before = R_0_5
//     Eigen::Matrix4d R_0_5 = R_0_6 * R_0_6_Neg;

//     // 3) Calculate J1 angle (next step)
//     // calc j1 angle
//     double x_r05 = R_0_5(0, 3);
//     double y_r05 = R_0_5(1, 3);

//     double theta1Rad = std::atan2(y_r05, x_r05); // atan2(y, x)
//     double theta1Deg = _radToDeg(theta1Rad);

//     // Set J1 angle mathematical zero for further calculations
//     // ? use (-theta1Rad) instead of theta1Rad: why? because we want to rotate back to zero
//     double x_at_j1_zero = x_r05 * cos(-theta1Rad) - y_r05 * sin(-theta1Rad);
//     // double y_at_j1_zero = y_r05 * cos(-theta1Rad) + x_r05 * sin(-theta1Rad);

//     double L1 = std::abs(x_at_j1_zero - _dhParams[0].a);
//     double L4 = R_0_5(2, 3) - _dhParams[0].d;                          // Z value of R_0_5 matrix - d value of J1
//     double L2 = sqrt(pow(L1, 2) + pow(L4, 2));                         // L2 = sqrt(L1^2 + L4^2)
//     double L3 = sqrt(pow(_dhParams[3].d, 2) + pow(_dhParams[2].a, 2)); // L3 = sqrt(d^2 + a^2)

//     // theha b = deg(atan(L1/L4)
//     double THETA_B = _radToDeg(std::atan2(L1, L4)); // atan2(L1, L4)

//     double THETA_C = _radToDeg(std::acos((pow(_dhParams[1].a, 2) + pow(L2, 2) - pow(L3, 2)) / (2 * _dhParams[1].a * L2)));

//     double THETA_D = _radToDeg(std::acos((pow(L3, 2) + pow(_dhParams[1].a, 2) - pow(L2, 2)) / (2 * L3 * _dhParams[1].a)));

//     double THETA_E = _radToDeg(std::atan2(_dhParams[2].a, _dhParams[3].d));

//     double theta2Deg;
//     if (x_at_j1_zero > _dhParams[0].a)
//     {
//         if (L4 > 0)
//         {
//             theta2Deg = THETA_B - THETA_C;
//         }
//         else
//         {
//             theta2Deg = THETA_B - THETA_C + 180.0f;
//         }
//     }
//     else
//     {
//         theta2Deg = -(THETA_B + THETA_C);
//     }

//     double theta3Deg = -(THETA_D + THETA_E) + 90.0f;

//     // Use J1, J2, J3 angles to find J3 orientation
//     Eigen::Matrix4d DH_J1 = _dhToTable(_dhParams[0], theta1Rad);
//     Eigen::Matrix4d DH_J2 = _dhToTable(_dhParams[1], _degToRad(theta2Deg), -90.0f);
//     Eigen::Matrix4d DH_J3 = _dhToTable(_dhParams[2], _degToRad(theta3Deg), 180.0f);

//     Eigen::Matrix4d R_0_3 = DH_J1 * DH_J2 * DH_J3; // R_0_3 matrix

//     // Merge overall rotation(R_0_6) with R_0_3 matrix
//     // Transpose R_0_3 matrix and multiply with R_0_6 matrix
//     Eigen::Matrix3d R_0_3_T = R_0_3.block<3, 3>(0, 0).transpose(); // Transpose of the upper-left 3x3 submatrix of R_0_3

//     // Calculate R_3_6 matrix (R_0_3_T * R_0_6<3,3>) -> actual rotation of spherical wrist
//     Eigen::Matrix3d R_3_6 = R_0_3_T * R_0_6.block<3, 3>(0, 0);
//     // extract J4, J5, J6 angles from R_3_6 matrix

//     // Determine if the wrist is flipped (J5 angle negative)
//     bool                flipWrist = false;
//     std::vector<double> angles    = getJointAnglesInRadOrDeg(1); // Get angles in radians
//     if (angles.size() == Utils::NUM_DOF)                         // ? Avoid out-of-bounds access – update this if more joints are added
//     {
//         flipWrist = angles[4] < 0;
//     }

//     double theta5Rad = std::acos(R_3_6(2, 2));
//     double theta4Rad, theta6Rad;

//     if (std::abs(theta5Rad) < 1e-6)
//     {
//         // Singularity: theta5 is ~0 → wrist is aligned → set default values
//         theta4Rad = 0;
//         theta6Rad = std::atan2(-R_3_6(0, 1), R_3_6(0, 0));
//     }
//     else
//     {
//         // Optional: flipWrist = true → Inverse ik-solution w. J5 negative
//         if (!flipWrist)
//         {
//             theta4Rad = std::atan2(R_3_6(1, 2), R_3_6(0, 2));
//             theta6Rad = std::atan2(R_3_6(2, 1), -R_3_6(2, 0));
//             // std::cout << "[INFO] Wrist not flipped – J5 positive.\n";
//         }
//         else
//         {
//             // Flip wrist: invert theta5 and negate directions for theta4/theta6
//             theta5Rad *= -1;
//             theta4Rad = std::atan2(-R_3_6(1, 2), -R_3_6(0, 2));
//             theta6Rad = std::atan2(-R_3_6(2, 1), R_3_6(2, 0));
//             // std::cout << "[INFO] Wrist flipped – J5 negative.\n";
//         }
//     }

//     // Debug output
//     // std::cout << std::fixed << std::setprecision(6);
//     // std::cout << "R_0_6_T:\n"
//     //           << R_0_6_T << std::endl;
//     // std::cout << "I_T_Toolframe:\n"
//     //           << I_T_Toolframe << std::endl;
//     // std::cout << "R_0_6:\n"
//     //           << R_0_6 << std::endl;
//     // std::cout << "R_0_6_Neg:\n"
//     //           << R_0_6_Neg << std::endl;
//     // std::cout << "R_0_5:\n"
//     //           << R_0_5 << std::endl;
//     // -----------------------Geometric values----------------------
//     // std::cout << "x_at_j1_zero: " << x_at_j1_zero << std::endl;
//     // std::cout << "y_at_j1_zero: " << y_at_j1_zero << std::endl;
//     // std::cout << "L1: " << L1 << std::endl;
//     // std::cout << "L2: " << L2 << std::endl;
//     // std::cout << "L3: " << L3 << std::endl;
//     // std::cout << "L4: " << L4 << std::endl;
//     // std::cout << "_dhParams[0].a: " << _dhParams[0].a << std::endl;
//     // std::cout << "_dhParams[0].d: " << _dhParams[0].d << std::endl;
//     // std::cout << "Theta B (deg): " << THETA_B << std::endl;
//     // std::cout << "Theta C (deg): " << THETA_C << std::endl;
//     // std::cout << "Theta D (deg): " << THETA_D << std::endl;
//     // std::cout << "Theta E (deg): " << THETA_E << std::endl;
//     // ------------------------Angles result------------------------
//     // std::cout << "Theta1 (deg): " << theta1Deg << std::endl;
//     // std::cout << "Theta2 (deg): " << theta2Deg << std::endl;
//     // std::cout << "Theta3 (deg): " << theta3Deg << std::endl;
//     // std::cout << "Theta4 (deg): " << _radToDeg(theta4Rad) << std::endl;
//     // std::cout << "Theta5 (deg): " << _radToDeg(theta5Rad) << std::endl;
//     // std::cout << "Theta6 (deg): " << _radToDeg(theta6Rad) << std::endl;
//     // std::cout << "DH_J1\n"
//     //           << DH_J1 << std::endl;
//     // std::cout << "DH_J2\n"
//     //           << DH_J2 << std::endl;
//     // std::cout << "DH_J3\n"
//     //           << DH_J3 << std::endl;
//     // std::cout << "R_0_3:\n"
//     //           << R_0_3 << std::endl;
//     // std::cout << "R_0_3_T:\n"
//     //           << R_0_3_T << std::endl;
//     // std::cout << "R_3_6:\n"
//     //           << R_3_6 << std::endl;

//     return {theta1Deg, theta2Deg, theta3Deg, _radToDeg(theta4Rad), _radToDeg(theta5Rad), _radToDeg(theta6Rad)};
// }

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Angles Kinematics::inverseKinematics(double x, double y, double z, double yaw, double pitch, double roll)
// {
//     // Serial.println("[IK] --------------------- NEW IK REQUEST ---------------------");
//     // Serial.print("[IK] Target Pose -> X: ");
//     // Serial.print(x);
//     // Serial.print(" Y: ");
//     // Serial.print(y);
//     // Serial.print(" Z: ");
//     // Serial.print(z);
//     // Serial.print(" Yaw: ");
//     // Serial.print(yaw);
//     // Serial.print(" Pitch: ");
//     // Serial.print(pitch);
//     // Serial.print(" Roll: ");
//     // Serial.println(roll);

// #ifdef DEBUG_IK
//     std::cout << std::fixed << std::setprecision(6);
//     std::cout << "[IK] --------------------- NEW IK REQUEST ---------------------\n";
//     std::cout << "[IK] Target Pose -> X: " << x
//               << " Y: " << y
//               << " Z: " << z
//               << " Yaw: " << yaw
//               << " Pitch: " << pitch
//               << " Roll: " << roll << std::endl;
// #endif
//     // Generate all valid IK solutions (filtered by joint limits)
//     std::vector<Angles> validSolutions = _generateAllIKSolutions(x, y, z, yaw, pitch, roll);

//     if (validSolutions.empty())
//     {
// #ifdef DEBUG_IK
//         std::cout << "[IK] No valid solution found – out of joint limits.\n";
// #endif
//         // Return an invalid solution (you can check this in higher-level code)
//         return {-1, -1, -1, -1, -1, -1};
//     }

//     Angles best = _selectBestSolution(validSolutions);
//     // Serial.println("[IK] --------------------- SELECTED SOLUTION ---------------------");
//     // Serial.print("J1: ");
//     // Serial.print(best.theta1);
//     // Serial.print(" J2: ");
//     // Serial.print(best.theta2);
//     // Serial.print(" J3: ");
//     // Serial.print(best.theta3);
//     // Serial.print(" J4: ");
//     // Serial.print(best.theta4);
//     // Serial.print(" J5: ");
//     // Serial.print(best.theta5);
//     // Serial.print(" J6: ");
//     // Serial.println(best.theta6);

// #ifdef DEBUG_IK
//     Angles best = _selectBestSolution(validSolutions);
//     std::cout << "[IK] --------------------- SELECTED SOLUTION ---------------------\n";
//     std::cout << "J1: " << best.theta1
//               << " J2: " << best.theta2
//               << " J3: " << best.theta3
//               << " J4: " << best.theta4
//               << " J5: " << best.theta5
//               << " J6: " << best.theta6 << std::endl;

//     return best;
// #endif
//     // Select the best solution (minimal distance to current joints)
//     return _selectBestSolution(validSolutions);
// }

// std::vector<Angles> Kinematics::_generateAllIKSolutions(double x, double y, double z, double yaw, double pitch, double roll)
// {
//     std::vector<Angles> solutions;
//     solutions.reserve(8);

//     // Variants: shoulder (J1), elbow (J3), wrist flip (J5)
//     std::vector<int>  shoulderVariants = {1, -1};       // 1 = default, -1 = alternate J1
//     std::vector<int>  elbowVariants    = {1, -1};       // 1 = elbow down, -1 = elbow up
//     std::vector<bool> wristVariants    = {false, true}; // false = normal, true = flipped

//     for (int s : shoulderVariants)
//     {
//         for (int e : elbowVariants)
//         {
//             for (bool w : wristVariants)
//             {
//                 Angles candidate = _computeIKSolution(x, y, z, yaw, pitch, roll, s, e, w);
//                 // Serial.println("[IK] --------------------- ALL SOLUTIONS ---------------------");
//                 // Serial.print("[IK] Candidate -> S: ");
//                 // Serial.print(s);
//                 // Serial.print(" E: ");
//                 // Serial.print(e);
//                 // Serial.print(" W: ");
//                 // Serial.print(w ? "Flipped" : "Normal");
//                 // Serial.print(" | Angles: [");
//                 // Serial.print(candidate.theta1);
//                 // Serial.print(", ");
//                 // Serial.print(candidate.theta2);
//                 // Serial.print(", ");
//                 // Serial.print(candidate.theta3);
//                 // Serial.print(", ");
//                 // Serial.print(candidate.theta4);
//                 // Serial.print(", ");
//                 // Serial.print(candidate.theta5);
//                 // Serial.print(", ");
//                 // Serial.print(candidate.theta6);
//                 // Serial.println("]");

// #ifdef DEBUG_IK
//                 std::cout << std::fixed << std::setprecision(6);
//                 std::cout << "[IK] --------------------- ALL SOLUTIONS ---------------------\n";
//                 std::cout << "[IK] Candidate -> S: " << s
//                           << " E: " << e
//                           << " W: " << (w ? "Flipped" : "Normal")
//                           << " | Angles: ["
//                           << candidate.theta1 << ", "
//                           << candidate.theta2 << ", "
//                           << candidate.theta3 << ", "
//                           << candidate.theta4 << ", "
//                           << candidate.theta5 << ", "
//                           << candidate.theta6 << "]\n";
// #endif
//                 if (_isSolutionValid(candidate))
//                 {
//                     solutions.push_back(candidate);
// #ifdef DEBUG_IK
//                     std::cout << "[IK] -> VALID (within joint limits)\n";

// #endif
//                 }
// #ifdef DEBUG_IK
//                 else
//                 {
//                     std::cout << "[IK] -> INVALID (joint limits exceeded)\n";
//                 }
// #endif
//             }
//         }
//     }
// #ifdef DEBUG_IK
//     std::cout << "[IK] Total valid solutions: " << solutions.size() << std::endl;
// #endif
//     return solutions;
// }

// Angles Kinematics::_computeIKSolution(double x, double y, double z,
//                                       double yaw, double pitch, double roll,
//                                       int shoulderVariant, int elbowVariant, bool flipWrist)
// {
//     // ----------------------- Transformation calculations -----------------------
//     Eigen::Matrix4d R_0_6_T       = _createTransformationMatrix(x, y, z, yaw, pitch, roll);
//     Eigen::Matrix4d I_T_Toolframe = _toolFrameMatrix.inverse();
//     Eigen::Matrix4d R_0_6         = R_0_6_T * I_T_Toolframe;

//     Eigen::Matrix4d R_0_6_Neg = Eigen::Matrix4d::Identity();
//     R_0_6_Neg(2, 3)           = -_dhParams[5].d;
//     Eigen::Matrix4d R_0_5     = R_0_6 * R_0_6_Neg;

//     // ----------------------- J1, J2, J3 calculations -----------------------
//     double x_r05 = R_0_5(0, 3);
//     double y_r05 = R_0_5(1, 3);

//     double theta1Rad = std::atan2(y_r05, x_r05);
//     if (shoulderVariant == -1)
//         theta1Rad += (theta1Rad > 0 ? -M_PI : M_PI); // switch shoulder side

//     double theta1Deg = _radToDeg(theta1Rad);

//     double x_at_j1_zero = x_r05 * cos(-theta1Rad) - y_r05 * sin(-theta1Rad);

//     double L1 = std::abs(x_at_j1_zero - _dhParams[0].a);
//     double L4 = R_0_5(2, 3) - _dhParams[0].d;
//     double L2 = sqrt(pow(L1, 2) + pow(L4, 2));
//     double L3 = sqrt(pow(_dhParams[3].d, 2) + pow(_dhParams[2].a, 2));

//     double THETA_B = _radToDeg(std::atan2(L1, L4));
//     double acosC   = _clamp01((pow(_dhParams[1].a, 2) + pow(L2, 2) - pow(L3, 2)) / (2 * _dhParams[1].a * L2));
//     double THETA_C = _radToDeg((elbowVariant == 1) ? std::acos(acosC) : -std::acos(acosC));

//     double acosD   = _clamp01((pow(L3, 2) + pow(_dhParams[1].a, 2) - pow(L2, 2)) / (2 * L3 * _dhParams[1].a));
//     double THETA_D = _radToDeg((elbowVariant == 1) ? std::acos(acosD) : -std::acos(acosD));
//     double THETA_E = _radToDeg(std::atan2(_dhParams[2].a, _dhParams[3].d));

//     double theta2Deg = (x_at_j1_zero > _dhParams[0].a)
//                            ? (L4 > 0 ? THETA_B - THETA_C : THETA_B - THETA_C + 180.0f)
//                            : -(THETA_B + THETA_C);

//     double theta3Deg = -(THETA_D + THETA_E) + 90.0f;

//     // ----------------------- J4, J5, J6 calculations -----------------------
//     Eigen::Matrix4d DH_J1 = _dhToTable(_dhParams[0], theta1Rad);
//     Eigen::Matrix4d DH_J2 = _dhToTable(_dhParams[1], _degToRad(theta2Deg), -90.0f);
//     Eigen::Matrix4d DH_J3 = _dhToTable(_dhParams[2], _degToRad(theta3Deg), 180.0f);

//     Eigen::Matrix4d R_0_3   = DH_J1 * DH_J2 * DH_J3;
//     Eigen::Matrix3d R_0_3_T = R_0_3.block<3, 3>(0, 0).transpose();
//     Eigen::Matrix3d R_3_6   = R_0_3_T * R_0_6.block<3, 3>(0, 0);

//     double theta5Rad = std::acos(_clamp01(R_3_6(2, 2)));
//     double theta4Rad, theta6Rad;

//     if (std::abs(theta5Rad) < 1e-6)
//     {
//         theta4Rad = 0;
//         theta6Rad = std::atan2(-R_3_6(0, 1), R_3_6(0, 0));
//     }
//     else
//     {
//         if (!flipWrist)
//         {
//             theta4Rad = std::atan2(R_3_6(1, 2), R_3_6(0, 2));
//             theta6Rad = std::atan2(R_3_6(2, 1), -R_3_6(2, 0));
//         }
//         else
//         {
//             theta5Rad *= -1;
//             theta4Rad = std::atan2(-R_3_6(1, 2), -R_3_6(0, 2));
//             theta6Rad = std::atan2(-R_3_6(2, 1), R_3_6(2, 0));
//         }
//     }

//     return {
//         theta1Deg,
//         theta2Deg,
//         theta3Deg,
//         _radToDeg(theta4Rad),
//         _radToDeg(theta5Rad),
//         _radToDeg(theta6Rad)};
// }

// bool Kinematics::_isSolutionValid(const Angles& angles) const
// {
//     if (_motorConfigs.size() < Utils::NUM_DOF)
//         return false;

//     const double candidate[6] = {
//         angles.theta1, angles.theta2, angles.theta3,
//         angles.theta4, angles.theta5, angles.theta6};

//     for (size_t i = 0; i < Utils::NUM_DOF; ++i)
//     {
//         const MotorConfig* cfg = _motorConfigs[i];
//         if (!cfg)
//             return false;

//         if (std::isnan(candidate[i]) || std::isinf(candidate[i]))
//             return false;

//         if (candidate[i] < cfg->minAngleDeg || candidate[i] > cfg->maxAngleDeg)
//             return false;
//     }

//     return true;
// }

// Angles Kinematics::_selectBestSolution(const std::vector<Angles>& solutions) const
// {
//     std::vector<double> currAngles = getJointAnglesInRadOrDeg(false); // false = deg

//     auto distance = [&](const Angles& a)
//     {
//         const double candidate[6] = {
//             a.theta1, a.theta2, a.theta3, a.theta4, a.theta5, a.theta6};

//         double dist = 0.0;
//         for (size_t i = 0; i < Utils::NUM_DOF; ++i)
//             dist += std::abs(candidate[i] - currAngles[i]);

//         return dist;
//     };

//     const Angles* best    = &solutions[0];
//     double        minDist = distance(solutions[0]);

//     for (size_t i = 1; i < solutions.size(); ++i)
//     {
//         double d = distance(solutions[i]);
//         if (d < minDist)
//         {
//             minDist = d;
//             best    = &solutions[i];
//         }
//     }

// #ifdef DEBUG_IK
//     std::cout << "[IK] Selected solution with distance: " << minDist << std::endl;
// #endif

//     return *best;
// }

Angles Kinematics::inverseKinematics(double x, double y, double z,
                                     double yaw, double pitch, double roll,
                                     bool forceCurrVariant)
{
#ifdef DEBUG_IK
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "[IK] --------------------- NEW IK REQUEST ---------------------\n";
    std::cout << "[IK] Target Pose -> X: " << x
              << " Y: " << y
              << " Z: " << z
              << " Yaw: " << yaw
              << " Pitch: " << pitch
              << " Roll: " << roll << std::endl;
#endif

    std::vector<Angles> validSolutions = _generateAllIKSolutions(x, y, z, yaw, pitch, roll);

    if (validSolutions.empty())
    {
#ifdef DEBUG_IK
        std::cout << "[IK] No valid solution found – out of joint limits.\n";
#endif
        return {-1, -1, -1, -1, -1, -1, 0, 0, false};
    }

    if (forceCurrVariant)
    {
        for (auto& sol : validSolutions)
        {
            if (_matchesCurrentVariant(sol))
            {
#ifdef DEBUG_IK
                std::cout << "[IK] Using forced current variant.\n";
#endif
                return sol;
            }
        }
        // Falls Variante nicht möglich -> fallback
#ifdef DEBUG_IK
        std::cout << "[IK] Forced variant not valid, falling back to best solution.\n";
#endif
        Angles best = _selectBestSolution(validSolutions);
        _updateCurrentVariant(best);
        return best;
    }

    Angles best = _selectBestSolution(validSolutions);
    _updateCurrentVariant(best);

#ifdef DEBUG_IK
    std::cout << "[IK] --------------------- SELECTED SOLUTION ---------------------\n";
    std::cout << "J1: " << best.theta1
              << " J2: " << best.theta2
              << " J3: " << best.theta3
              << " J4: " << best.theta4
              << " J5: " << best.theta5
              << " J6: " << best.theta6 << std::endl;
#endif
    return best;
}

void Kinematics::setCurrentVariant(int s, int e, bool w)
{
    _currentVariant = {s, e, w};
}

std::vector<Angles> Kinematics::_generateAllIKSolutions(double x, double y, double z,
                                                        double yaw, double pitch, double roll)
{
    std::vector<Angles> solutions;
    solutions.reserve(8);

    std::vector<int>  shoulderVariants = {1, -1};
    std::vector<int>  elbowVariants    = {1, -1};
    std::vector<bool> wristVariants    = {false, true};

    for (int s : shoulderVariants)
    {
        for (int e : elbowVariants)
        {
            for (bool w : wristVariants)
            {
                Angles candidate = _computeIKSolution(x, y, z, yaw, pitch, roll, s, e, w);

#ifdef DEBUG_IK
                std::cout << std::fixed << std::setprecision(6);
                std::cout << "[IK] --------------------- ALL SOLUTIONS ---------------------\n";
                std::cout << "[IK] Candidate -> S: " << s
                          << " E: " << e
                          << " W: " << (w ? "Flipped" : "Normal")
                          << " | Angles: ["
                          << candidate.theta1 << ", "
                          << candidate.theta2 << ", "
                          << candidate.theta3 << ", "
                          << candidate.theta4 << ", "
                          << candidate.theta5 << ", "
                          << candidate.theta6 << "]\n";
#endif

                if (_isSolutionValid(candidate))
                {
                    solutions.push_back(candidate);
#ifdef DEBUG_IK
                    std::cout << "[IK] -> VALID (within joint limits)\n";
#endif
                }
#ifdef DEBUG_IK
                else
                {
                    std::cout << "[IK] -> INVALID (joint limits exceeded)\n";
                }
#endif
            }
        }
    }

#ifdef DEBUG_IK
    std::cout << "[IK] Total valid solutions: " << solutions.size() << std::endl;
#endif
    return solutions;
}

Angles Kinematics::_computeIKSolution(double x, double y, double z,
                                      double yaw, double pitch, double roll,
                                      int shoulderVariant, int elbowVariant, bool flipWrist)
{
    Eigen::Matrix4d R_0_6_T       = _createTransformationMatrix(x, y, z, yaw, pitch, roll);
    Eigen::Matrix4d I_T_Toolframe = _toolFrameMatrix.inverse();
    Eigen::Matrix4d R_0_6         = R_0_6_T * I_T_Toolframe;

    Eigen::Matrix4d R_0_6_Neg = Eigen::Matrix4d::Identity();
    R_0_6_Neg(2, 3)           = -_dhParams[5].d;
    Eigen::Matrix4d R_0_5     = R_0_6 * R_0_6_Neg;

    double x_r05 = R_0_5(0, 3);
    double y_r05 = R_0_5(1, 3);

    double theta1Rad = std::atan2(y_r05, x_r05);
    if (shoulderVariant == -1)
        theta1Rad += (theta1Rad > 0 ? -M_PI : M_PI);

    double theta1Deg = _radToDeg(theta1Rad);

    double x_at_j1_zero = x_r05 * cos(-theta1Rad) - y_r05 * sin(-theta1Rad);

    double L1 = std::abs(x_at_j1_zero - _dhParams[0].a);
    double L4 = R_0_5(2, 3) - _dhParams[0].d;
    double L2 = sqrt(pow(L1, 2) + pow(L4, 2));
    double L3 = sqrt(pow(_dhParams[3].d, 2) + pow(_dhParams[2].a, 2));

    double THETA_B = _radToDeg(std::atan2(L1, L4));
    double acosC   = _clamp01((pow(_dhParams[1].a, 2) + pow(L2, 2) - pow(L3, 2)) / (2 * _dhParams[1].a * L2));
    double THETA_C = _radToDeg((elbowVariant == 1) ? std::acos(acosC) : -std::acos(acosC));

    double acosD   = _clamp01((pow(L3, 2) + pow(_dhParams[1].a, 2) - pow(L2, 2)) / (2 * L3 * _dhParams[1].a));
    double THETA_D = _radToDeg((elbowVariant == 1) ? std::acos(acosD) : -std::acos(acosD));
    double THETA_E = _radToDeg(std::atan2(_dhParams[2].a, _dhParams[3].d));

    double theta2Deg = (x_at_j1_zero > _dhParams[0].a)
                           ? (L4 > 0 ? THETA_B - THETA_C : THETA_B - THETA_C + 180.0f)
                           : -(THETA_B + THETA_C);

    double theta3Deg = -(THETA_D + THETA_E) + 90.0f;

    Eigen::Matrix4d DH_J1 = _dhToTable(_dhParams[0], theta1Rad);
    Eigen::Matrix4d DH_J2 = _dhToTable(_dhParams[1], _degToRad(theta2Deg), -90.0f);
    Eigen::Matrix4d DH_J3 = _dhToTable(_dhParams[2], _degToRad(theta3Deg), 180.0f);

    Eigen::Matrix4d R_0_3   = DH_J1 * DH_J2 * DH_J3;
    Eigen::Matrix3d R_0_3_T = R_0_3.block<3, 3>(0, 0).transpose();
    Eigen::Matrix3d R_3_6   = R_0_3_T * R_0_6.block<3, 3>(0, 0);
    // !!!!!!
    // Serial.println("R_3_6 Orientation Matrix:");
    // for (int i = 0; i < 3; ++i)
    // {
    //     Serial.print("Row ");
    //     Serial.print(i);
    //     Serial.print(": ");
    //     Serial.print(R_3_6(i, 0), 4);
    //     Serial.print("\t");
    //     Serial.print(R_3_6(i, 1), 4);
    //     Serial.print("\t");
    //     Serial.println(R_3_6(i, 2), 4);
    // }
    // Eigen::Vector3d zAxisTool = R_3_6.col(2);
    // Serial.print("Tool-Z-Axis → X: ");
    // Serial.print(zAxisTool.x(), 4);
    // Serial.print(" Y: ");
    // Serial.print(zAxisTool.y(), 4);
    // Serial.print(" Z: ");
    // Serial.println(zAxisTool.z(), 4);
    // !!!!!!

    double theta5Rad = std::acos(_clamp01(R_3_6(2, 2)));
    double theta4Rad, theta6Rad;

    if (std::abs(theta5Rad) < 1e-6)
    {
        theta4Rad = 0;
        theta6Rad = std::atan2(-R_3_6(0, 1), R_3_6(0, 0));
    }
    else
    {
        if (!flipWrist)
        {
            theta4Rad = std::atan2(R_3_6(1, 2), R_3_6(0, 2));
            theta6Rad = std::atan2(R_3_6(2, 1), -R_3_6(2, 0));
        }
        else
        {
            theta5Rad *= -1;
            theta4Rad = std::atan2(-R_3_6(1, 2), -R_3_6(0, 2));
            theta6Rad = std::atan2(-R_3_6(2, 1), R_3_6(2, 0));
        }
    }

    return {
        theta1Deg, theta2Deg, theta3Deg,
        _radToDeg(theta4Rad),
        _radToDeg(theta5Rad),
        _radToDeg(theta6Rad),
        shoulderVariant,
        elbowVariant,
        flipWrist};
}

bool Kinematics::_isSolutionValid(const Angles& angles) const
{
    if (_motorConfigs.size() < Utils::NUM_DOF)
        return false;

    const double candidate[6] = {
        angles.theta1, angles.theta2, angles.theta3,
        angles.theta4, angles.theta5, angles.theta6};

    for (size_t i = 0; i < Utils::NUM_DOF; ++i)
    {
        const MotorConfig* cfg = _motorConfigs[i];
        if (!cfg)
            return false;

        if (std::isnan(candidate[i]) || std::isinf(candidate[i]))
            return false;

        if (candidate[i] < cfg->minAngleDeg || candidate[i] > cfg->maxAngleDeg)
            return false;
    }

    return true;
}

Angles Kinematics::_selectBestSolution(const std::vector<Angles>& solutions) const
{
    std::vector<double> currAngles = getJointAnglesInRadOrDeg(false);

    auto distance = [&](const Angles& a)
    {
        const double candidate[6] = {
            a.theta1, a.theta2, a.theta3, a.theta4, a.theta5, a.theta6};

        double dist = 0.0;
        for (size_t i = 0; i < Utils::NUM_DOF; ++i)
            dist += std::abs(candidate[i] - currAngles[i]);

        return dist;
    };

    const Angles* best    = &solutions[0];
    double        minDist = distance(solutions[0]);

    for (size_t i = 1; i < solutions.size(); ++i)
    {
        double d = distance(solutions[i]);
        if (d < minDist)
        {
            minDist = d;
            best    = &solutions[i];
        }
    }

#ifdef DEBUG_IK
    std::cout << "[IK] Selected solution with distance: " << minDist << std::endl;
#endif

    return *best;
}

bool Kinematics::_matchesCurrentVariant(const Angles& sol) const
{
    return sol.shoulderVariant == _currentVariant.shoulder &&
           sol.elbowVariant == _currentVariant.elbow &&
           sol.wristFlipped == _currentVariant.wristFlip;
}

void Kinematics::_updateCurrentVariant(const Angles& best)
{
    _currentVariant.shoulder  = best.shoulderVariant;
    _currentVariant.elbow     = best.elbowVariant;
    _currentVariant.wristFlip = best.wristFlipped;
}
