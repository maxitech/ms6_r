/**
 * @file Kinematics.h
 * @brief Contains the Kinematics class for forward and inverse kinematics calculations.
 */

#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "Compat.h"
#include "MotorConfig.h"
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
    double roll, pitch, yaw; ///< Orientation (Euler angles) in degrees
    bool   inSingularity;    ///< Indicates if the pose is in a singularity
};

/**
 * @brief Joint angles of a 6-DOF robotic arm.
 */
struct Angles
{
    double theta1;
    double theta2;
    double theta3;
    double theta4;
    double theta5;
    double theta6;
};

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
    Kinematics(const std::vector<MotorConfig*>& motorConfigs, const std::vector<DHparam>& dhParams);

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
     * @return Joint angles to achieve the pose.
     */
    Angles inverseKinematics(double x, double y, double z, double yaw, double pitch, double roll);

private:
    double _totalRatio(const MotorConfig* motorConfig) const;
    double _stepsPerRev(const MotorConfig* motorConfig) const;
    double _stepsToDeg(const MotorConfig* motorConfig, const int currPosInSteps) const;
    int    _degToSteps(const MotorConfig* motorConfig, const double deg) const;
    double _degToRad(const double deg) const;
    double _radToDeg(const double rad) const;

    /**
     * @brief Creates transformation matrix from DH parameters.
     * @param param DH parameters for the joint.
     * @param theta Current joint angle (rad).
     * @param offsetDeg Optional angle offset (deg).
     * @return Transformation matrix.
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
     */
    Eigen::Matrix4d _createTransformationMatrix(double x, double y, double z, double yaw, double pitch, double roll) const;

    // Helper function
    bool _isValidConfig(const MotorConfig* cfg) const;

    // Note: Kineaatics class does not manage the memory of MotorConfig pointers.
    // Setup class is responsible for creating and managin MotorConfig objects.
    const std::vector<MotorConfig*>& _motorConfigs;                                  ///< Motor configuration for each joint
    const std::vector<DHparam>       _dhParams;                                      ///< DH parameters for each joint
    Eigen::Matrix4d                  _toolFrameMatrix = Eigen::Matrix4d::Identity(); ///< Tool frame transformation matrix
};

#endif // KINEMATICS_H
