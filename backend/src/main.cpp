#include "AxisData.h"
#include "AxisGroup.h"
#include "CommandProcessor.h"
#include "Homing.h"
#include "HomingState.h"
#include "LimitSwitches.h"
#include "ProgramLoader.h"
#include "SerialHandler.h"
#include "Utils.h"
#include "teensystep4.h"
#include "timers/Teensy4/TMR/TMR.h"
#include <Arduino.h>
#include <memory>

using namespace TS4;

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
const int STANDBY_POS_J1        = 40'000;

const int HOMING_VELOCITY_J2    = -20'000;
const int MOVE_AWAY_VELOCITY_J2 = 4000;
const int MOVE_BACK_VELOCITY_J2 = -2000;
const int STANDBY_POS_J2        = 55'000;

const int HOMING_VELOCITY_J3    = 5'000;
const int MOVE_AWAY_VELOCITY_J3 = -800;
const int MOVE_BACK_VELOCITY_J3 = 400;
const int STANDBY_POS_J3        = -20'000;

const int HOMING_VELOCITY_J4    = 4000;
const int MOVE_AWAY_VELOCITY_J4 = -800;
const int MOVE_BACK_VELOCITY_J4 = 400;
const int STANDBY_POS_J4        = -24'000;

const int HOMING_VELOCITY_J5    = 5000;
const int MOVE_AWAY_VELOCITY_J5 = -400;
const int MOVE_BACK_VELOCITY_J5 = 200;
const int STANDBY_POS_J5        = -22'000;

const int HOMING_VELOCITY_J6    = 2000;
const int MOVE_AWAY_VELOCITY_J6 = -200;
const int MOVE_BACK_VELOCITY_J6 = 200;
const int STANDBY_POS_J6        = -6400;

// Initialize Classes
LimitSwitches    limitSwitches(ledPin, limitSwitchPins); // Manages the state of limit switches
ProgramLoader    programLoader(limitSwitches);           // Controls loading programs and state management
CommandProcessor cmdProcessor(programLoader);            // Parses and processes incoming commands
SerialHandler    serialHandler;                          // Handles serial communication and command routing
Homing           homingManager(limitSwitchPins);         // Manages the homing process for multiple axes
Stepper          motorJ1(motorJ1Step, motorJ1Dir);       // int stepPin, int dirPin
Stepper          motorJ2(motorJ2Step, motorJ2Dir);
Stepper          motorJ3(motorJ3Step, motorJ3Dir);
Stepper          motorJ4(motorJ4Step, motorJ4Dir);
Stepper          motorJ5(motorJ5Step, motorJ5Dir);
Stepper          motorJ6(motorJ6Step, motorJ6Dir);

AxisData axis1 = {MOVE_TO_SWITCH, &motorJ1, J1, HOMING_VELOCITY_J1, MOVE_AWAY_VELOCITY_J1, MOVE_BACK_VELOCITY_J1, STANDBY_POS_J1};
AxisData axis2 = {MOVE_TO_SWITCH, &motorJ2, J2, HOMING_VELOCITY_J2, MOVE_AWAY_VELOCITY_J2, MOVE_BACK_VELOCITY_J2, STANDBY_POS_J2};
AxisData axis3 = {MOVE_TO_SWITCH, &motorJ3, J3, HOMING_VELOCITY_J3, MOVE_AWAY_VELOCITY_J3, MOVE_BACK_VELOCITY_J3, STANDBY_POS_J3};
AxisData axis4 = {MOVE_TO_SWITCH, &motorJ4, J4, HOMING_VELOCITY_J4, MOVE_AWAY_VELOCITY_J4, MOVE_BACK_VELOCITY_J4, STANDBY_POS_J4};
AxisData axis5 = {MOVE_TO_SWITCH, &motorJ5, J5, HOMING_VELOCITY_J5, MOVE_AWAY_VELOCITY_J5, MOVE_BACK_VELOCITY_J5, STANDBY_POS_J5};
AxisData axis6 = {MOVE_TO_SWITCH, &motorJ6, J6, HOMING_VELOCITY_J6, MOVE_AWAY_VELOCITY_J6, MOVE_BACK_VELOCITY_J6, STANDBY_POS_J6};

void setup()
{
    while (!Serial)
    {
        // wait
    };
    Serial.println("Setup started");
    delay(20);

    pinMode(motorJ1En, OUTPUT);
    pinMode(motorJ2En, OUTPUT);
    pinMode(motorJ3En, OUTPUT);
    pinMode(motorJ4En, OUTPUT);
    pinMode(motorJ5En, OUTPUT);
    pinMode(motorJ6En, OUTPUT);

    // digitalWrite(motorJ1En, HIGH);
    // digitalWrite(motorJ2En, HIGH);
    // digitalWrite(motorJ3En, HIGH);
    // digitalWrite(motorJ4En, HIGH);
    // digitalWrite(motorJ5En, HIGH);
    // digitalWrite(motorJ6En, HIGH);

    TS4::begin();
    TimerFactory::attachModule(new TMRModule<0>());

    motorJ1
        .setMaxSpeed(50'000)
        .setAcceleration(26'000);

    motorJ2
        .setMaxSpeed(160'000)
        .setAcceleration(50'000);

    motorJ3
        .setMaxSpeed(60'000)
        .setAcceleration(30'000);

    motorJ4
        .setMaxSpeed(40'000)
        .setAcceleration(30'000);

    motorJ5
        .setMaxSpeed(40'000)
        .setAcceleration(20'000);

    motorJ6
        .setMaxSpeed(15'000)
        .setAcceleration(8000);

    // Create and add groups
    auto group1 = std::make_unique<AxisGroup>();
    group1->addAxis(&axis1);
    group1->addAxis(&axis2);
    group1->addAxis(&axis4);

    auto group2 = std::make_unique<AxisGroup>();
    group2->addAxis(&axis3);

    auto group3 = std::make_unique<AxisGroup>();
    group3->addAxis(&axis5);
    group3->addAxis(&axis6);

    homingManager.addGroup(std::move(group1));
    homingManager.addGroup(std::move(group2));
    homingManager.addGroup(std::move(group3));

    limitSwitches.init();
    serialHandler.setCommandProcessor(&cmdProcessor);

    Serial.println("Setup done");
    delay(20);
}

void loop()
{

    serialHandler.listenForSerial();
    programLoader.run();
    // homingManager.executeHoming();
}
