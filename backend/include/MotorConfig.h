#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H
#include "teensystep4.h"

using namespace TS4;

struct MotorConfig
{
    Stepper* motor;
    int      stepsPerRev;
    int      microsteps;
    float    driverTeeth;
    float    drivenTeeth;
    float    gearboxRatio;

    MotorConfig(Stepper& motor, int spr, int ms, float drt = 1.0f, float dnt = 1.0f, float gr = 1.0f)
        : motor(&motor)
        , stepsPerRev(spr)
        , microsteps(ms)
        , driverTeeth(drt)
        , drivenTeeth(dnt)
        , gearboxRatio(gr)
    {
    }
};

#endif // MOTORCONFIG_H