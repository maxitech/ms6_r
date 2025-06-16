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
    static Setup&  getInstance(); // Static method for instance-access
    void           init();
    SerialHandler& getSerialHandler() { return serialHandler; }
    ProgramLoader& getProgramLoader() { return programLoader; }
    void           update(const String& jsonString);

private:
    Setup();                                 // Prevents direct object creation
    Setup(const Setup&)            = delete; // Do not allow copies
    Setup& operator=(const Setup&) = delete; //  Do not allow assignments

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
