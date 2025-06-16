#ifndef SETUP_H
#define SETUP_H

#include "AxisData.h"
#include "AxisGroup.h"
#include "CommandProcessor.h"
#include "Homing.h"
#include "HomingState.h"
#include "Kinematics.h"
#include "LimitSwitches.h"
#include "MotorConfig.h"
#include "ProgramLoader.h"
#include "SerialHandler.h"
#include "teensystep4.h"
#include "timers/Teensy4/TMR/TMR.h"
#include <Arduino.h>

#include "Kinematics.h"
#include <ArduinoJson.h>

class Setup
{
public:
    explicit Setup(const String& jsonString);

private:
    void                 _validateJson();
    std::vector<DHparam> _extractDHParams();
    std::vector<int>     _extractHomingParams();

    bool _checkExists(const JsonObjectConst& obj, const char* name);
    bool _checkFields(const char* f1, const char* f2, const char* f3, const char* f4, const char* context);

    JsonDocument _jsonDoc;
    bool         _valid = false;
    const String _jsonStr;
};

#endif // SETUP_H
