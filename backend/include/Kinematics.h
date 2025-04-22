#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "MotorConfig.h"
#include <Arduino.h>

class Kinematics
{
public:
    explicit Kinematics(std::vector<MotorConfig>& motorConfigs) {};
};

#endif // KINEMATICS_H