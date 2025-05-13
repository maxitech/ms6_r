#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "Compat.h"
#include "MotorConfig.h"
#include <cmath>

struct DHparam
{
    double a;     ///< Link length
    double alpha; ///< Link twist
    double d;     ///< Link offset
};

struct Pose
{
    double x, y, z;          ///< Position (mm)
    double roll, pitch, yaw; ///< Orientation (Euler angles) in degrees
    bool   inSingularity;    ///< Indicates if the pose is in a singularity
};

struct Angles
{
    double theta1;
    double theta2;
    double theta3;
    double theta4;
    double theta5;
    double theta6;
};

class Kinematics
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Kinematics(const std::vector<MotorConfig>& motorConfigs, const std::vector<DHparam>& dhParams);

    std::vector<double> getJointAnglesInRad() const;
    void                setToolFrame(double x, double y, double z, double yaw, double pitch, double roll);
    Pose                forwardKinematics();
    Angles              inverseKinematics(double x, double y, double z, double yaw, double pitch, double roll);
    std::vector<Angles> calculateAllIKSolutions(double x, double y, double z, double yaw, double pitch, double roll);

private:
    double _totalRatio(const MotorConfig& motorConfig) const;
    double _stepsPerRev(const MotorConfig& motorConfig) const;
    double _stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const;
    int    _degToSteps(const MotorConfig& motorConfig, const double deg) const;
    double _degToRad(const double deg) const;
    double _radToDeg(const double rad) const;

    Eigen::Matrix4d _dhToTable(const DHparam& param, double theta, double offsetDeg = 0) const;
    Eigen::Matrix4d _createTransformationMatrix(double x, double y, double z, double yaw, double pitch, double roll) const;

    const std::vector<MotorConfig> _motorConfigs;
    const std::vector<DHparam>     _dhParams;
    Eigen::Matrix4d                _toolFrameMatrix = Eigen::Matrix4d::Identity();
};

#endif // KINEMATICS_H
