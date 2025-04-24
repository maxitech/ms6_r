#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "MotorConfig.h"
#include <Arduino.h>
#include <ArduinoEigenDense.h>
#include <cmath>

struct DHparam
{
    float a;     ///< Link length
    float alpha; ///< Link twist
    float d;     ///< Link offset
    // float theta; ///< Joint angle (dynamic parameter)
};

class Kinematics
{
public:
    Kinematics(const std::vector<MotorConfig>& motorConfigs, const std::vector<DHparam>& dhParams);

    std::vector<float> getJointAnglesInRad() const;
    std::vector<float> forwardKinematics();

private:
    float _totalRatio(const MotorConfig& motorConfig) const;
    float _stepsPerRev(const MotorConfig& motorConfig) const;
    float _stepsToDeg(const MotorConfig& motorConfig, const int currPosInSteps) const;
    int   _degToSteps(const MotorConfig& motorConfig, const float deg) const;
    float _degToRad(const float deg) const;
    float _radToDeg(const float rad) const;

    Eigen::Matrix4f _dhToTable(const DHparam& param, const float theta) const;

    const std::vector<MotorConfig>& _motorConfigs;
    const std::vector<DHparam>&     _dhParams;
    Eigen::Matrix4f                 _toolFrameMatrix = Eigen::Matrix4f::Identity();
};

#endif // KINEMATICS_H