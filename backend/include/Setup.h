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
    String       _jsonStr;

    // ************Setup*************
    // Init variables
    const byte ledPin = 13;

    // Limit switches
    const byte                limitJ1         = 0;
    const byte                limitJ2         = 1;
    const byte                limitJ3         = 2;
    const byte                limitJ4         = 3;
    const byte                limitJ5         = 4;
    const byte                limitJ6         = 5;
    const std::array<byte, 6> limitSwitchPins = {limitJ1, limitJ2, limitJ3, limitJ4, limitJ5, limitJ6};

    // Motor pins
    const int motorJ1En   = 6;
    const int motorJ1Step = 9;
    const int motorJ1Dir  = 10;

    const int motorJ2En   = 11;
    const int motorJ2Step = 12;
    const int motorJ2Dir  = 24;

    const int motorJ3En   = 25;
    const int motorJ3Step = 26;
    const int motorJ3Dir  = 27;

    const int motorJ4En   = 28;
    const int motorJ4Step = 29;
    const int motorJ4Dir  = 30;

    const int motorJ5En   = 38;
    const int motorJ5Step = 37;
    const int motorJ5Dir  = 36;

    const int motorJ6En   = 35;
    const int motorJ6Step = 34;
    const int motorJ6Dir  = 33;

    // Homing parameters for each axis, negative values indicate direction(CCW)
    const int HOMING_VELOCITY_J1    = -6000; // 3k
    const int MOVE_AWAY_VELOCITY_J1 = 800;
    const int MOVE_BACK_VELOCITY_J1 = -200;
    const int HOME_POS_J1           = 40'000;

    const int HOMING_VELOCITY_J2    = -20'000;
    const int MOVE_AWAY_VELOCITY_J2 = 4000;
    const int MOVE_BACK_VELOCITY_J2 = -2000;
    const int HOME_POS_J2           = 55'000;

    const int HOMING_VELOCITY_J3    = 5'000;
    const int MOVE_AWAY_VELOCITY_J3 = -800;
    const int MOVE_BACK_VELOCITY_J3 = 400;
    const int HOME_POS_J3           = -20'000;

    const int HOMING_VELOCITY_J4    = 4000;
    const int MOVE_AWAY_VELOCITY_J4 = -800;
    const int MOVE_BACK_VELOCITY_J4 = 400;
    const int HOME_POS_J4           = -24'000;

    const int HOMING_VELOCITY_J5    = 5000;
    const int MOVE_AWAY_VELOCITY_J5 = -400;
    const int MOVE_BACK_VELOCITY_J5 = 200;
    const int HOME_POS_J5           = -22'000;

    const int HOMING_VELOCITY_J6    = 2000;
    const int MOVE_AWAY_VELOCITY_J6 = -200;
    const int MOVE_BACK_VELOCITY_J6 = 200;
    const int HOME_POS_J6           = -6400;

    // //*********** Initialize Classes *********//
    Stepper motorJ1, motorJ2, motorJ3, motorJ4, motorJ5, motorJ6; // int stepPin, int dirPin

    // Initialize MotorConfigs
    MotorConfig              mCfg1        = {&motorJ1, 200, 4, 16.0f, 100.0f, 1.0f};
    MotorConfig              mCfg2        = {&motorJ2, 200, 4, 16.0f, 80.0f, 14.0f};
    MotorConfig              mCfg3        = {&motorJ3, 200, 4, 16.0f, 100.0f, 1.0f};
    MotorConfig              mCfg4        = {&motorJ4, 400, 4, 16.0f, 60.0f, 1.0f};
    MotorConfig              mCfg5        = {&motorJ5, 200, 4, 16.0f, 32.0f, 1.0f};
    MotorConfig              mCfg6        = {&motorJ6, 200, 4, 1.0f, 1.0f, 1.0f};
    std::vector<MotorConfig> motorConfigs = {mCfg1, mCfg2, mCfg3, mCfg4, mCfg5, mCfg6};

    // Initialize Kinematics with DH parameters
    std::vector<DHparam> dhParams = {
        {37.5f, -1.571f, 135.300f},
        {160.0f, 0.0f, 0.0f, -90.0f},
        {-15.0f, 1.571f, 0.0f, 180.0f},
        {0.0f, -1.571f, 138.400f},
        {0.0f, 1.571f, 0.0f},
        {0.0f, 0.0f, 29.270f}};

    LimitSwitches    limitSwitches; // Manages the state of limit switches
    Homing           homingManager; // Manages the homing process for multiple axes
    ProgramLoader    programLoader; // Controls loading programs and state management
    CommandProcessor cmdProcessor;  // Parses and processes incoming commands
    SerialHandler    serialHandler; // Handles serial communication and command routing
    Kinematics*      kin = nullptr; // Kinematics calculations for the robot arm

    AxisData axis1 = {MOVE_TO_SWITCH, &motorJ1, J1, HOMING_VELOCITY_J1, MOVE_AWAY_VELOCITY_J1, MOVE_BACK_VELOCITY_J1, HOME_POS_J1};
    AxisData axis2 = {MOVE_TO_SWITCH, &motorJ2, J2, HOMING_VELOCITY_J2, MOVE_AWAY_VELOCITY_J2, MOVE_BACK_VELOCITY_J2, HOME_POS_J2};
    AxisData axis3 = {MOVE_TO_SWITCH, &motorJ3, J3, HOMING_VELOCITY_J3, MOVE_AWAY_VELOCITY_J3, MOVE_BACK_VELOCITY_J3, HOME_POS_J3};
    AxisData axis4 = {MOVE_TO_SWITCH, &motorJ4, J4, HOMING_VELOCITY_J4, MOVE_AWAY_VELOCITY_J4, MOVE_BACK_VELOCITY_J4, HOME_POS_J4};
    AxisData axis5 = {MOVE_TO_SWITCH, &motorJ5, J5, HOMING_VELOCITY_J5, MOVE_AWAY_VELOCITY_J5, MOVE_BACK_VELOCITY_J5, HOME_POS_J5};
    AxisData axis6 = {MOVE_TO_SWITCH, &motorJ6, J6, HOMING_VELOCITY_J6, MOVE_AWAY_VELOCITY_J6, MOVE_BACK_VELOCITY_J6, HOME_POS_J6};
};

#endif // SETUP_H
