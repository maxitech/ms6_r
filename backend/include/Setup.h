/**
 * @file Setup.h
 * @brief This file contains the Setup class which initializes and manages the robot's hardware and software components.
 * @details It provides a centralized location for managing the robot's setup and configuration.
 */

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
#include "Utils.h"
#include <ArduinoJson.h>

/**
 * @brief This struct represents a motion profile for the robot's motors.
 */
struct MotionProfile
{
    int max_speed; ///< Maximum speed for the motor in steps
    int accel;     ///< Acceleration for the motor in steps
};

/**
 * @brief Singleton class for initializing and accessing core robot components
 */
class Setup
{
public:
    /**
     * @brief Returns the singleton instance of the Setup class.
     * @return Reference to the Setup instance.
     */
    static Setup& getInstance(); // Static method for instance-access

    /**
     * @brief Initializes hardware and software components.
     * @details Should be called once from the Arduino `setup()` function.
     */
    void init();

    /**
     * @brief Getter function which returns the SerialHandler instance.
     * @return Reference to the SerialHandler instance.
     */
    SerialHandler& getSerialHandler() { return _serialHandler; }

    /**
     * @brief Getter function which returns the ProgramLoader instance.
     * @return Reference to the ProgramLoader instance.
     */
    ProgramLoader& getProgramLoader() { return _programLoader; }

    /**
     * @brief Returns a pointer to the robot's kinematics instance.
     * @details Provides access to forward and inverse kinematics computations,
     * allowing other modules to query or manipulate joint and pose data.
     * @return Pointer to the Kinematics instance.
     */
    Kinematics* getKinematics() const { return _kin; };

    void update(const String& jsonString);

private:
    // ************Singleton*************
    Setup();                                 // Prevents direct object creation
    Setup(const Setup&)            = delete; // Do not allow copies
    Setup& operator=(const Setup&) = delete; //  Do not allow assignments

    /**
     * @brief Validates the JSON string.
     */
    void _validateJson();

    /**
     * @brief Extracts the dh parameters from the JSON document.
     * @return An array of DHparam structs.
     */
    std::array<DHparam, Utils::NUM_DOF> _extractDHParams();

    /**
     * @brief Extracts the homing parameters(for now only home pos) from the JSON document.
     * @return An array of home positions.
     */
    std::array<int, Utils::NUM_DOF> _extractHomingParams();

    /**
     * @brief Extracts the motion profiles from the JSON document.
     * @return An array of MotionProfile structs.
     */
    std::array<MotionProfile, Utils::NUM_DOF> _extractMotionProfiles();

    /**
     * @brief Checks if a given part exists in the JSON object.
     * @param obj The JSON object to check.
     * @param name The name of the field to check.
     * @return True if the field exists, false otherwise.
     */
    bool _checkExists(const JsonObjectConst& obj, const char* name);

    /**
     * @brief Checks if the required fields are present in the JSON object.
     * @param f1 The name of the first field to check.
     * @param f2 The name of the second field to check.
     * @param f3 The name of the third field to check.
     * @param f4 The name of the fourth field to check.
     * @param context A string describing the context of the check (for error messages).
     * @return True if all fields exist, false otherwise.
     */
    bool _checkFields(const char* f1, const char* f2, const char* f3, const char* f4, const char* context);

    /**
     * @brief Updates the DH parameters based on the JSON document.
     */
    void _updateDhParams();

    /**
     * @brief Updates the home positions based on the JSON document.
     */
    void _updateHomePositions();

    /**
     * @brief Updates the motion profiles based on the JSON document.
     */
    void _updateMotionProfiles();

    /**
     * @brief Updates the motor configurations based on the JSON document.
     */
    void _updateMotorConfigs();

    /**
     * @brief Updates the kinematics instance with the current motor configurations and DH parameters.
     */
    void _updateKinematics();

    /**
     * @brief Updates the axis data for each axis.
     */
    void _updateAxisData();

    /**
     * @brief Updates the homing groups with the current axis data.
     */
    void _updateHomingGroups();

    // ************Variables*************
    JsonDocument _jsonDoc;       ///< JSON document for storing configuration data
    bool         _valid = false; ///< Flag to indicate if the JSON document is valid
    String       _jsonStr;       ///< String representation of the JSON document received from the frontend

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

    const int _HOMING_VELOCITY_J1    = -3000;
    const int _MOVE_AWAY_VELOCITY_J1 = 400;
    const int _MOVE_BACK_VELOCITY_J1 = -200;
    int       _HOME_POS_J1;
    // const int HOME_POS_J1           = 40'000;

    const int _HOMING_VELOCITY_J2    = -10'000;
    const int _MOVE_AWAY_VELOCITY_J2 = 2000;
    const int _MOVE_BACK_VELOCITY_J2 = -1000;
    int       _HOME_POS_J2;
    // const int HOME_POS_J2           = 55'000;

    const int _HOMING_VELOCITY_J3    = 2'000;
    const int _MOVE_AWAY_VELOCITY_J3 = -400;
    const int _MOVE_BACK_VELOCITY_J3 = 200;
    int       _HOME_POS_J3;
    // const int HOME_POS_J3           = -20'000;

    const int _HOMING_VELOCITY_J4    = 4000;
    const int _MOVE_AWAY_VELOCITY_J4 = -800;
    const int _MOVE_BACK_VELOCITY_J4 = 400;
    int       _HOME_POS_J4;
    // const int HOME_POS_J4           = -24'000;

    const int _HOMING_VELOCITY_J5    = 5000;
    const int _MOVE_AWAY_VELOCITY_J5 = -500;
    const int _MOVE_BACK_VELOCITY_J5 = 250;
    int       _HOME_POS_J5;
    // const int HOME_POS_J5           = -22'000;

    const int _HOMING_VELOCITY_J6    = 2500;
    const int _MOVE_AWAY_VELOCITY_J6 = -400;
    const int _MOVE_BACK_VELOCITY_J6 = 200;
    int       _HOME_POS_J6;
    // const int HOME_POS_J6           = -6400;

    // Note: min values have to be positive
    const float _minAngleDegJ1 = 152;
    const float _minAngleDegJ2 = 88;
    const float _minAngleDegJ3 = 50;
    const float _minAngleDegJ4 = 180;
    const float _minAngleDegJ5 = 110;
    const float _minAngleDegJ6 = 180;

    // Note: max values have to be negative
    const float _maxAngleDegJ1 = -180;
    const float _maxAngleDegJ2 = -14;
    const float _maxAngleDegJ3 = -57;
    const float _maxAngleDegJ4 = -166;
    const float _maxAngleDegJ5 = -120;
    const float _maxAngleDegJ6 = -174;

    // //*********** Initialize Classes *********//

    /**
     * @brief Stepper motor instances.
     */
    Stepper _motorJ1, _motorJ2, _motorJ3, _motorJ4, _motorJ5, _motorJ6; // int stepPin, int dirPin

    // Initialize MotorConfigs
    std::vector<MotorConfig*> _motorConfigs; ///< Vector of MotorConfig pointers @internal

    // Initialize Kinematics with DH parameters
    std::array<DHparam, Utils::NUM_DOF> _dhParams; ///< Array of DH parameters for the robot arm
    // *Default values for L position of MS6R below as reference
    // std::vector<DHparam> dhParams = {
    //     {37.5f, -1.571f, 135.300f},
    //     {160.0f, 0.0f, 0.0f, -90.0f},
    //     {-15.0f, 1.571f, 0.0f, 180.0f},
    //     {0.0f, -1.571f, 138.400f},
    //     {0.0f, 1.571f, 0.0f},
    //     {0.0f, 0.0f, 29.270f}};

    LimitSwitches    _limitSwitches; ///< Manages the state of limit switches
    Homing           _homingManager; ///< Manages the homing process for multiple axes
    ProgramLoader    _programLoader; ///< Controls loading programs and state management
    CommandProcessor _cmdProcessor;  ///< Parses and processes incoming commands
    SerialHandler    _serialHandler; ///< Handles serial communication and command routing
    Kinematics*      _kin = nullptr; ///< Kinematics calculations for the robot arm

    AxisData* _axis1 = nullptr; ///< Data for axis 1
    AxisData* _axis2 = nullptr; ///< Data for axis 2
    AxisData* _axis3 = nullptr; ///< Data for axis 3
    AxisData* _axis4 = nullptr; ///< Data for axis 4
    AxisData* _axis5 = nullptr; ///< Data for axis 5
    AxisData* _axis6 = nullptr; ///< Data for axis 6
};

#endif // SETUP_H
