#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "Compat.h"
#include "MotorConfig.h"
#include <cmath>

struct DHparam
{
    float a;     ///< Link length
    float alpha; ///< Link twist
    float d;     ///< Link offset
    // float theta; ///< Joint angle (dynamic parameter)
};

struct Pose
{
    float x, y, z;          ///< Position (mm)
    float roll, pitch, yaw; ///< Orientation (Euler angles) in degrees
};

class Kinematics
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Kinematics(const std::vector<MotorConfig>& motorConfigs, const std::vector<DHparam>& dhParams);

    std::vector<float> getJointAnglesInRad() const;
    Pose               forwardKinematics();

private:
    float _totalRatio(const MotorConfig& motorConfig) const;
    float _stepsPerRev(const MotorConfig& motorConfig) const;
    float _stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const;
    int   _degToSteps(const MotorConfig& motorConfig, const float deg) const;
    float _degToRad(const float deg) const;
    float _radToDeg(const float rad) const;

    Eigen::Matrix4f _dhToTable(const DHparam& param, const float theta) const;

    const std::vector<MotorConfig> _motorConfigs;
    const std::vector<DHparam>     _dhParams;
    Eigen::Matrix4f                _toolFrameMatrix = Eigen::Matrix4f::Identity();
};

#endif // KINEMATICS_H