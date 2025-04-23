#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

struct MotorConfig
{
    int   stepsPerRev;
    int   microsteps;
    float driverTeeth;
    float drivenTeeth;
    float gearboxRatio;

    MotorConfig(int spr, int ms, float drt = 1.0f, float dnt = 1.0f, float gr = 1.0f)
        : stepsPerRev(spr)
        , microsteps(ms)
        , driverTeeth(drt)
        , drivenTeeth(dnt)
        , gearboxRatio(gr)
    {
    }
};

#endif // MOTORCONFIG_H