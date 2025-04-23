#include "Kinematics.h"

Kinematics::Kinematics(std::vector<MotorConfig>& motorConfigs)
    : _motorConfigs(motorConfigs)
{
    for (auto& motorConfig : _motorConfigs)
    {
    }
}

float Kinematics::_calcAngle(const MotorConfig& _motorConfig, const int currPosInSteps) const
{
    float gearRatio        = _motorConfig.drivenTeeth / _motorConfig.driverTeeth;
    float totalRatio       = _motorConfig.gearboxRatio * gearRatio;
    float totalStepsPerRev = _motorConfig.stepsPerRev * _motorConfig.microsteps * totalRatio;
    float angle            = (static_cast<float>(currPosInSteps) / totalStepsPerRev) * 360.0f;
    return angle;
}