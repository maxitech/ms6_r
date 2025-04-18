#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

struct MotorConfig
{
    int   stepsPerRev;
    int   microsteps;
    float driverTeeth;
    float drivenTeeth;
    float gearboxRatio;
};

#endif // MOTORCONFIG_H