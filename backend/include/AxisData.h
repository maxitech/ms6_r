#ifndef AXISDATA_H
#define AXISDATA_H

#include "HomingState.h"
#include "teensystep4.h"
#include <Arduino.h>

using namespace TS4;

enum Axes
{
    J1,
    J2,
    J3,
    J4,
    J5,
    J6
};

struct AxisData
{
    HomingState homingState;
    Stepper*    motor;
    Axes        axis;
    int         HOMING_VELOCITY;
    int         MOVE_AWAY_VELOCITY;
    int         MOVE_BACK_VELOCITY;
    int         STANDBY_POS;
    bool        isHomingDone = false;
};

#endif // AXISDATA_H