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

struct MotionProfile
{
    int max_speed;
    int accel;
};

class Setup
{
public:
    static Setup&  getInstance(); // Static method for instance-access
    void           init();
    SerialHandler& getSerialHandler() { return _serialHandler; }
    ProgramLoader& getProgramLoader() { return _programLoader; }
    Kinematics*    getKinematics() const { return _kin; }
    void           update(const String& jsonString);

private:
    Setup();                                 // Prevents direct object creation
    Setup(const Setup&)            = delete; // Do not allow copies
    Setup& operator=(const Setup&) = delete; //  Do not allow assignments

    void                       _validateJson();
    std::vector<DHparam>       _extractDHParams();
    std::vector<int>           _extractHomingParams();
    std::vector<MotionProfile> _extractMotionProfiles();

    bool _checkExists(const JsonObjectConst& obj, const char* name);
    bool _checkFields(const char* f1, const char* f2, const char* f3, const char* f4, const char* context);

    JsonDocument _jsonDoc;
    bool         _valid = false;
    String       _jsonStr;

    // ************Setup*************
    // Init variables
    const byte _ledPin = 13;

    // Limit switches
    const byte                _limitJ1         = 0;
    const byte                _limitJ2         = 1;
    const byte                _limitJ3         = 2;
    const byte                _limitJ4         = 3;
    const byte                _limitJ5         = 4;
    const byte                _limitJ6         = 5;
    const std::array<byte, 6> _limitSwitchPins = {_limitJ1, _limitJ2, _limitJ3, _limitJ4, _limitJ5, _limitJ6};

    // Motor pins
    const int _motorJ1En   = 6;
    const int _motorJ1Step = 9;
    const int _motorJ1Dir  = 10;

    const int _motorJ2En   = 11;
    const int _motorJ2Step = 12;
    const int _motorJ2Dir  = 24;

    const int _motorJ3En   = 25;
    const int _motorJ3Step = 26;
    const int _motorJ3Dir  = 27;

    const int _motorJ4En   = 28;
    const int _motorJ4Step = 29;
    const int _motorJ4Dir  = 30;

    const int _motorJ5En   = 38;
    const int _motorJ5Step = 37;
    const int _motorJ5Dir  = 36;

    const int _motorJ6En   = 35;
    const int _motorJ6Step = 34;
    const int _motorJ6Dir  = 33;

    // Homing parameters for each axis, negative values indicate direction(CCW)

    const int _HOMING_VELOCITY_J1    = -6000; // 3k
    const int _MOVE_AWAY_VELOCITY_J1 = 800;
    const int _MOVE_BACK_VELOCITY_J1 = -200;
    int       _HOME_POS_J1;
    // const int HOME_POS_J1           = 40'000;

    const int _HOMING_VELOCITY_J2    = -20'000;
    const int _MOVE_AWAY_VELOCITY_J2 = 4000;
    const int _MOVE_BACK_VELOCITY_J2 = -2000;
    int       _HOME_POS_J2;
    // const int HOME_POS_J2           = 55'000;

    const int _HOMING_VELOCITY_J3    = 5'000;
    const int _MOVE_AWAY_VELOCITY_J3 = -800;
    const int _MOVE_BACK_VELOCITY_J3 = 400;
    int       _HOME_POS_J3;
    // const int HOME_POS_J3           = -20'000;

    const int _HOMING_VELOCITY_J4    = 4000;
    const int _MOVE_AWAY_VELOCITY_J4 = -800;
    const int _MOVE_BACK_VELOCITY_J4 = 400;
    int       _HOME_POS_J4;
    // const int HOME_POS_J4           = -24'000;

    const int _HOMING_VELOCITY_J5    = 8000;
    const int _MOVE_AWAY_VELOCITY_J5 = -800;
    const int _MOVE_BACK_VELOCITY_J5 = 400;
    int       _HOME_POS_J5;
    // const int HOME_POS_J5           = -22'000;

    const int _HOMING_VELOCITY_J6    = 4000;
    const int _MOVE_AWAY_VELOCITY_J6 = -400;
    const int _MOVE_BACK_VELOCITY_J6 = 400;
    int       _HOME_POS_J6;
    // const int HOME_POS_J6           = -6400;

    // //*********** Initialize Classes *********//
    Stepper _motorJ1, _motorJ2, _motorJ3, _motorJ4, _motorJ5, _motorJ6; // int stepPin, int dirPin

    // Initialize MotorConfigs
    std::vector<MotorConfig*> _motorConfigs; // Vector of MotorConfig pointers

    // Initialize Kinematics with DH parameters
    std::vector<DHparam> _dhParams;
    // *Default values for L position of MS6R below as reference
    // std::vector<DHparam> dhParams = {
    //     {37.5f, -1.571f, 135.300f},
    //     {160.0f, 0.0f, 0.0f, -90.0f},
    //     {-15.0f, 1.571f, 0.0f, 180.0f},
    //     {0.0f, -1.571f, 138.400f},
    //     {0.0f, 1.571f, 0.0f},
    //     {0.0f, 0.0f, 29.270f}};

    LimitSwitches    _limitSwitches; // Manages the state of limit switches
    Homing           _homingManager; // Manages the homing process for multiple axes
    ProgramLoader    _programLoader; // Controls loading programs and state management
    CommandProcessor _cmdProcessor;  // Parses and processes incoming commands
    SerialHandler    _serialHandler; // Handles serial communication and command routing
    Kinematics*      _kin = nullptr; // Kinematics calculations for the robot arm

    AxisData* _axis1 = nullptr;
    AxisData* _axis2 = nullptr;
    AxisData* _axis3 = nullptr;
    AxisData* _axis4 = nullptr;
    AxisData* _axis5 = nullptr;
    AxisData* _axis6 = nullptr;
};

#endif // SETUP_H
