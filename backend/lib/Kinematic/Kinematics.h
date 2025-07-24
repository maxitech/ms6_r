/**
 * @file Kinematics.h
 * @brief Contains the Kinematics class for forward and inverse kinematics calculations.
 */

#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "Compat.h"
#include "MotorConfig.h"
#include "Utils.h"
#include <cmath>

/**
 * @brief Denavit-Hartenberg parameters for a single joint.
 */
struct DHparam
{
    double a;                 ///< Link length
    double alpha;             ///< Link twist (in radians)
    double d;                 ///< Link offset
    double thetaOffset = 0.0; ///< Joint angle offset (in degrees)
};

/**
 * @brief Represents a 3D pose with position and orientation.
 */
struct Pose
{
    double x, y, z;          ///< Position (mm)
    double yaw, pitch, roll; ///< Orientation (Euler angles) in degrees
    bool   inSingularity;    ///< Indicates if the pose is in a singularity
};

/**
 * @brief Joint angles of a 6-DOF robotic arm.
 */
struct Angles
{
    double theta1, theta2, theta3, theta4, theta5, theta6;
    int    shoulderVariant; // 1 / -1
    int    elbowVariant;    // 1 / -1
    bool   wristFlipped;    // true / false
};

//!!!!!!!!!!!!!!!!!!!!
struct IKVariant
{
    int  shoulder;  // 1 oder -1
    int  elbow;     // 1 oder -1
    bool wristFlip; // false = normal, true = flipped
};

//!!!!

/**
 * @class Kinematics
 * @brief Handles forward and inverse kinematics for a 6-DOF robotic arm.
 */
class Kinematics
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    /**
     * @brief Constructs a Kinematics object.
     * @param motorConfigs Motor configuration list.
     * @param dhParams Denavit-Hartenberg parameters.
     */
    Kinematics(const std::vector<MotorConfig*>& motorConfigs, const std::array<DHparam, Utils::NUM_DOF>& dhParams);

    /**
     * @brief Returns current joint angles in radians.
     * @param radOrDeg 1 for radians, 0 for degrees.
     * @return Vector of joint angles (rad || deg).
     */
    std::vector<double> getJointAnglesInRadOrDeg(const int radOrDeg) const;

    /**
     * @brief Sets the tool frame transformation matrix.
     * @param x X position (mm)
     * @param y Y position (mm)
     * @param z Z position (mm)
     * @param yaw Yaw angle (deg)
     * @param pitch Pitch angle (deg)
     * @param roll Roll angle (deg)
     */
    void setToolFrame(double x, double y, double z, double yaw, double pitch, double roll);

    /**
     * @brief Computes the forward kinematics.
     * @return Pose of the end effector.
     */
    Pose forwardKinematics();

    /**
     * @brief Computes inverse kinematics for the given pose.
     * @param x X position (mm)
     * @param y Y position (mm)
     * @param z Z position (mm)
     * @param yaw Yaw angle (deg)
     * @param pitch Pitch angle (deg)
     * @param roll Roll angle (deg)
     * @param forceCurrVariant Bool indicating if variant of shoulder, elbow, wrist should be forced
     * @return Joint angles to achieve the pose.
     */
    Angles inverseKinematics(double x, double y, double z, double yaw, double pitch, double roll, bool forceCurrVariant);

    /**
     * @brief Converts degrees to steps for the given motor configuration.
     * @param motorConfig Pointer to the MotorConfig object.
     * @param deg Angle in degrees.
     * @return Position in steps.
     */
    int degToSteps(const MotorConfig* motorConfig, const double deg) const;

    //!!!!!
    void setCurrentVariant(int s, int e, bool w);
    //!!!

private:
    /**
     * @brief Computes the total ratio of the motor configuration.
     * @param motorConfig Pointer to the MotorConfig object.
     * @return Total ratio as a double.
     * @internal
     */
    double _totalRatio(const MotorConfig* motorConfig) const;

    /**
     * @brief Computes the number of steps per revolution for the given motor configuration.
     * @param motorConfig Pointer to the MotorConfig object.
     * @return Steps per revolution as a double.
     * @internal
     */
    double _stepsPerRev(const MotorConfig* motorConfig) const;

    /**
     * @brief Converts steps to degrees for the given motor configuration.
     * @param motorConfig Pointer to the MotorConfig object.
     * @param currPosInSteps Current position in steps.
     * @return Position in degrees.
     * @internal
     */
    double _stepsToDeg(const MotorConfig* motorConfig, const int currPosInSteps) const;

    /**
     * @brief Converts degrees to radians.
     * @param deg Angle in degrees.
     * @return Angle in radians.
     * @internal
     */
    double _degToRad(const double deg) const;

    /**
     * @brief Converts radians to degrees.
     * @param rad Angle in radians.
     * @return Angle in degrees.
     * @internal
     */
    double _radToDeg(const double rad) const;

    /**
     * @brief Creates transformation matrix from DH parameters.
     * @param param DH parameters for the joint.
     * @param theta Current joint angle (rad).
     * @param offsetDeg Optional angle offset (deg).
     * @return Transformation matrix.
     * @internal
     */
    Eigen::Matrix4d _dhToTable(const DHparam& param, double theta, double offsetDeg = 0) const;

    /**
     * @brief Creates transformation matrix from position and orientation.
     * @param x X position (mm)
     * @param y Y position (mm)
     * @param z Z position (mm)
     * @param yaw Yaw angle (deg)
     * @param pitch Pitch angle (deg)
     * @param roll Roll angle (deg)
     * @return Homogeneous transformation matrix.
     * @internal
     */
    Eigen::Matrix4d _createTransformationMatrix(double x, double y, double z, double yaw, double pitch, double roll) const;

    //!!!!!!!!!!!prototype 1 fk
    Eigen::Matrix4d computeForwardKinematicsMatrix(const std::vector<double>& anglesRad);
    Eigen::MatrixXd computeNumericalJacobian(const std::vector<double>& anglesRad);
    //!!!

    // //!!!!!!! prototype 1 ik
    // std::vector<Angles> _generateAllIKSolutions(double x, double y, double z, double yaw, double pitch, double roll);
    // Angles              _computeIKSolution(double x, double y, double z, double yaw, double pitch, double roll,
    //                                        int shoulderVariant, int elbowVariant, bool flipWrist);
    // bool                _isSolutionValid(const Angles& angles) const;
    // Angles              _selectBestSolution(const std::vector<Angles>& solutions) const;

    // //!!!!!!

    //!!!!!!! prototype 2 ik
    std::vector<Angles> _generateAllIKSolutions(double x, double y, double z, double yaw, double pitch, double roll);
    Angles              _computeIKSolution(double x, double y, double z, double yaw, double pitch, double roll,
                                           int shoulderVariant, int elbowVariant, bool flipWrist);
    bool                _isSolutionValid(const Angles& angles) const;
    Angles              _selectBestSolution(const std::vector<Angles>& solutions) const;
    bool                _matchesCurrentVariant(const Angles& sol) const;
    void                _updateCurrentVariant(const Angles& best);

    IKVariant _currentVariant = {1, 1, false}; // Default
    //!!!!!!

    // Helper function
    bool _isValidConfig(const MotorConfig* cfg) const;

    // Note: Kinematics class does not manage the memory of MotorConfig pointers.
    // Setup class is responsible for creating and managing MotorConfig objects.
    const std::vector<MotorConfig*>&          _motorConfigs;                                  ///< Motor configuration for each joint
    const std::array<DHparam, Utils::NUM_DOF> _dhParams;                                      ///< DH parameters for each joint
    Eigen::Matrix4d                           _toolFrameMatrix = Eigen::Matrix4d::Identity(); ///< Tool frame transformation matrix
};

#endif // KINEMATICS_H
