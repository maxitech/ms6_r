#include <Arduino.h>
#include "SerialHandler.h"
#include "CommandProcessor.h"
#include "ProgramLoader.h"
#include "LimitSwitches.h"
#include "teensystep4.h"
#include "timers/Teensy4/TMR/TMR.h"

#include "Utils.h"

using namespace TS4;

// Init variables
const byte ledPin = 13;

// Limit switches
const byte limitJ1 = 0;
const byte limitJ2 = 1;
const byte limitJ3 = 2;
const byte limitJ4 = 3;
const byte limitJ5 = 4;
const byte limitJ6 = 5;
const std::array<byte, 6> limitSwitchPins = {limitJ1, limitJ2, limitJ3, limitJ4, limitJ5, limitJ6};

// Motor pins
const int motorJ1En = 6;
const int motorJ1Step = 9;
const int motorJ1Dir = 10;

const int motorJ2En = 11;
const int motorJ2Step = 12;
const int motorJ2Dir = 24;

const int motorJ3En = 25;
const int motorJ3Step = 26;
const int motorJ3Dir = 27;

const int motorJ4En = 28;
const int motorJ4Step = 29;
const int motorJ4Dir = 30;

const int motorJ5En = 38;
const int motorJ5Step = 37;
const int motorJ5Dir = 36;

const int motorJ6En = 35;
const int motorJ6Step = 34;
const int motorJ6Dir = 33;


enum Axes {
    J1,
    J2,
    J3,
    J4,
    J5,
    J6
};

enum HomingState {
    MOVE_TO_SWITCH,
    MOVE_AWAY_FROM_SWITCH,
    MOVE_BACK_TO_SWITCH,
    SET_ZERO_POINT,
    MOVE_TO_STANDBY_POS,
    COMPLETE
};

HomingState homingStateJ1 = MOVE_TO_SWITCH;
HomingState homingStateJ2 = MOVE_TO_SWITCH;


// Initialize Classes
LimitSwitches limitSwitches(ledPin, limitSwitchPins); // Manages the state of limit switches
ProgramLoader programLoader(limitSwitches);           // Controls loading programs and state management
CommandProcessor cmdProcessor(programLoader);         // Parses and processes incoming commands
SerialHandler serialHandler;                          // Handles serial communication and command routing

Stepper motorJ1(motorJ1Step, motorJ1Dir); // int stepPin, int dirPin
Stepper motorJ2(motorJ2Step, motorJ2Dir);
Stepper motorJ3(motorJ3Step, motorJ3Dir);
Stepper motorJ4(motorJ4Step, motorJ4Dir);
Stepper motorJ5(motorJ5Step, motorJ5Dir);
Stepper motorJ6(motorJ6Step, motorJ6Dir);


// Homing parameters for each axis, negative values indicate direction(CCW)
const int HOMING_VELOCITY_J1 = -5000;
const int MOVE_AWAY_SPEED_J1 = 200;
const int MOVE_BACK_SPEED_J1 = -100;
const int STANDBY_POS_J1 = 40000;

const int HOMING_VELOCITY_J2 = -10000;
const int MOVE_AWAY_SPEED_J2 = 2000;
const int MOVE_BACK_SPEED_J2 = -1000;
const int STANDBY_POS_J2 = 40000;


struct AxisData {
    HomingState homingState;
    Stepper* motor;
    Axes axis;
    int HOMING_VELOCITY;
    int MOVE_AWAY_SPEED;
    int MOVE_BACK_SPEED;
    int STANDBY_POS;
    bool isHomingDone;
};

AxisData axesData[] = {
    {MOVE_TO_SWITCH, &motorJ1, J1, HOMING_VELOCITY_J1, MOVE_AWAY_SPEED_J1, MOVE_BACK_SPEED_J1, STANDBY_POS_J1, false},
    {MOVE_TO_SWITCH, &motorJ2, J2, HOMING_VELOCITY_J2, MOVE_AWAY_SPEED_J2, MOVE_BACK_SPEED_J2, STANDBY_POS_J2, false}
};


uint8_t getActiveSwitches();
void handleSwitchContact();
bool debounceRead(byte pin);
void updateSwitchStatus(Axes axis, bool isActive);
void homeAxis(bool isCurrentlyActive, bool wasPreviouslyActive, AxisData& axisData);


void setup() { 
  Serial.begin(9600);
  while(!Serial);
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

  digitalWrite(motorJ1En, LOW);
  digitalWrite(motorJ2En, LOW);
  digitalWrite(motorJ3En, LOW);
  digitalWrite(motorJ4En, LOW);
  digitalWrite(motorJ5En, LOW);
  digitalWrite(motorJ6En, LOW);
  
  TS4::begin();
  TimerFactory::attachModule(new TMRModule<0>());
  
  motorJ1.setMaxSpeed(10000);
  motorJ1.setAcceleration(3000);

  motorJ2.setMaxSpeed(20000);
  motorJ2.setAcceleration(2000);

  motorJ3.setMaxSpeed(800);
  motorJ3.setAcceleration(500);

  motorJ4.setMaxSpeed(800);
  motorJ4.setAcceleration(500);

  motorJ5.setMaxSpeed(500);
  motorJ5.setAcceleration(300);

  motorJ6.setMaxSpeed(200);
  motorJ6.setAcceleration(500);
  
  limitSwitches.init();
  // serialHandler.setCommandProcessor(&cmdProcessor);

  Serial.println("Setup done");
}


void loop() {
  // serialHandler.listenForSerial();
  // programLoader.run();
//   if (motorJ1.isMoving || motorJ2.isMoving)
//   {
//     delay(10);
//   }
  
  handleSwitchContact();
}


uint8_t getActiveSwitches() {
  uint8_t activeSwitches = 0;
  for(byte i = 0; i < limitSwitchPins.size(); i++) {
    if(debounceRead(limitSwitchPins[i])) {
        activeSwitches |= (1 << i);
    }
  }
  return activeSwitches;
} 


bool debounceRead(byte pin) {
  //   static unsigned long lastDebounceTime[limitSwitchPins.size()] = {0}; 
  //   if (digitalRead(pin) == LOW) {
  //       if(Utils::nonBlockingDelay(100, lastDebounceTime[pin])) {
  //           return digitalRead(pin) == LOW;
  //       }
  //   }
  //   return false;
  // }
  if (digitalRead(pin) == LOW) {
      delay(5); // debounce time -> adjust if needed (5ms is a good starting point) **curr. blocking fix later if needed**
      return digitalRead(pin) == LOW;
  }
  return false;
}


uint8_t previousSwitchStatus = 0; // Holds the previous state of the switches
void handleSwitchContact() {
    uint8_t activeSwitches = getActiveSwitches(); // Aktuelle Schalterzustände

    for (byte i = 0; i < sizeof(axesData) / sizeof(axesData[i]); i++){
        bool isCurrentlyActive = activeSwitches & (1 << axesData[i].axis); // Current state (use axis enum val to check bit position)
        bool wasPreviouslyActive = previousSwitchStatus & (1 << axesData[i].axis); // Previous state

        // Pass the data to the homeAxis function
        homeAxis(isCurrentlyActive, wasPreviouslyActive, axesData[i]);
    }
    previousSwitchStatus = activeSwitches; // Update the previous switch status for the next iteration
}


uint8_t switchStatus = 0; // Status for all 6 switches default: 0=(all inactive)
void updateSwitchStatus(Axes axis, bool active) {
    if(active) {
        switchStatus |= (1 << axis);
    } else {
        switchStatus &= ~(1 << axis);
    }
}


void homeAxis(bool isCurrentlyActive, bool wasPreviouslyActive, AxisData& axisData) {
    // Extracting data from the AxisData struct
    HomingState& homingStateJ_x = axisData.homingState;
    Stepper& motorJ_x = *(axisData.motor);
    Axes axis_x = axisData.axis;
    int HOMING_VELOCITY = axisData.HOMING_VELOCITY;
    int MOVE_AWAY_SPEED = axisData.MOVE_AWAY_SPEED;
    int MOVE_BACK_SPEED = axisData.MOVE_BACK_SPEED;
    int STANDBY_POS = axisData.STANDBY_POS;
    
    
    switch (homingStateJ_x) {
        case MOVE_TO_SWITCH:
            if(isCurrentlyActive && !wasPreviouslyActive) {
                Serial.println(String(axis_x) + " pressed");
                motorJ_x.emergencyStop();
                updateSwitchStatus(axis_x, true);
                homingStateJ_x = MOVE_AWAY_FROM_SWITCH;
            } else {
                motorJ_x.rotateAsync(HOMING_VELOCITY);  // Move towards the switch
            }
            break;

        case MOVE_AWAY_FROM_SWITCH:
            if(!isCurrentlyActive && wasPreviouslyActive) {
                Serial.println(String(axis_x) + " released");
                motorJ_x.emergencyStop();
                updateSwitchStatus(axis_x, false);
                homingStateJ_x = MOVE_BACK_TO_SWITCH;    
            } else {
                motorJ_x.rotateAsync(MOVE_AWAY_SPEED);   // Move away from the switch
            }
            break;

        case MOVE_BACK_TO_SWITCH:
            if(isCurrentlyActive && !wasPreviouslyActive) {
                Serial.println(String(axis_x) + " pressed again");
                motorJ_x.emergencyStop();
                updateSwitchStatus(axis_x, true);
                homingStateJ_x = SET_ZERO_POINT;
            } else {
                motorJ_x.rotateAsync(MOVE_BACK_SPEED);  // Move back towards the switch again
            }
            break;

        case SET_ZERO_POINT: 
            Serial.println(String(axis_x) + " set zero point");
            motorJ_x.setPosition(0); // Set the current position to zero
            homingStateJ_x = MOVE_TO_STANDBY_POS;
            break;

        case MOVE_TO_STANDBY_POS:
            
            if(!motorJ_x.isMoving) {
                if(motorJ_x.getPosition() != STANDBY_POS) {
                    Serial.println(String(axis_x) + " move to operating position");
                    motorJ_x.moveAbsAsync(STANDBY_POS); // Move to a safe position after homing
                } else {
                Serial.println(String(axis_x) + " reached operating position");
                homingStateJ_x = COMPLETE;
                }
            }
            
            if (!isCurrentlyActive && wasPreviouslyActive) {
                Serial.println(String(axis_x) + " released while moving to operating position");
                updateSwitchStatus(axis_x, false);
            } 
            break;

        case COMPLETE:
            // Serial.println(String(axis_x) + " homing complete");
            axisData.isHomingDone = true; // Set the homing done flag to true
            break;
        }
}

