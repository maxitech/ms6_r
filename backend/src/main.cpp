#include <Arduino.h>
#include <memory>
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
const int HOMING_VELOCITY_J1 = -6000; //3k
const int MOVE_AWAY_VELOCITY_J1 = 800;
const int MOVE_BACK_VELOCITY_J1 = -200;
const int STANDBY_POS_J1 = 40'000;

const int HOMING_VELOCITY_J2 = -20'000;
const int MOVE_AWAY_VELOCITY_J2 = 4000;
const int MOVE_BACK_VELOCITY_J2 = -2000;
const int STANDBY_POS_J2 = 55'000;

const int HOMING_VELOCITY_J3 = 5'000;
const int MOVE_AWAY_VELOCITY_J3 = -200;
const int MOVE_BACK_VELOCITY_J3 = 100;
const int STANDBY_POS_J3 = -20'000;

const int HOMING_VELOCITY_J4 = 4000;
const int MOVE_AWAY_VELOCITY_J4 = -400;
const int MOVE_BACK_VELOCITY_J4 = 200;
const int STANDBY_POS_J4 = -24'000;

const int HOMING_VELOCITY_J5 = 4000;
const int MOVE_AWAY_VELOCITY_J5 = -400;
const int MOVE_BACK_VELOCITY_J5 = 200;
const int STANDBY_POS_J5 = -20'000;

const int HOMING_VELOCITY_J6 = 2000;
const int MOVE_AWAY_VELOCITY_J6 = -200;
const int MOVE_BACK_VELOCITY_J6 = 200;
const int STANDBY_POS_J6 = -6200;



// Global
uint8_t PREVIOUS_SWITCH_STATUS = 0; // Holds the previous state of the switches


struct AxisData {
    HomingState homingState;
    Stepper* motor;
    Axes axis;
    int HOMING_VELOCITY;
    int MOVE_AWAY_VELOCITY;
    int MOVE_BACK_VELOCITY;
    int STANDBY_POS;
    bool isHomingDone = false;
};

AxisData axis1 = {MOVE_TO_SWITCH, &motorJ1, J1, HOMING_VELOCITY_J1, MOVE_AWAY_VELOCITY_J1, MOVE_BACK_VELOCITY_J1, STANDBY_POS_J1};
AxisData axis2 = {MOVE_TO_SWITCH, &motorJ2, J2, HOMING_VELOCITY_J2, MOVE_AWAY_VELOCITY_J2, MOVE_BACK_VELOCITY_J2, STANDBY_POS_J2};
AxisData axis3 = {MOVE_TO_SWITCH, &motorJ3, J3, HOMING_VELOCITY_J3, MOVE_AWAY_VELOCITY_J3, MOVE_BACK_VELOCITY_J3, STANDBY_POS_J3};
AxisData axis4 = {MOVE_TO_SWITCH, &motorJ4, J4, HOMING_VELOCITY_J4, MOVE_AWAY_VELOCITY_J4, MOVE_BACK_VELOCITY_J4, STANDBY_POS_J4};
AxisData axis5 = {MOVE_TO_SWITCH, &motorJ5, J5, HOMING_VELOCITY_J5, MOVE_AWAY_VELOCITY_J5, MOVE_BACK_VELOCITY_J5, STANDBY_POS_J5};
AxisData axis6 = {MOVE_TO_SWITCH, &motorJ6, J6, HOMING_VELOCITY_J6, MOVE_AWAY_VELOCITY_J6, MOVE_BACK_VELOCITY_J6, STANDBY_POS_J6};



uint8_t getActiveSwitches();
bool debounceRead(byte pin);
void updateSwitchStatus(Axes axis, bool isActive);
void homeAxis(bool isCurrentlyActive, bool wasPreviouslyActive, AxisData& axisData);


struct AxisGroup {
    std::vector<AxisData*> axes;
    bool isGroupHomed = false;
    uint8_t groupPreviousSwitchStatus = 0; // Track switches locally for the group

    void addAxis(AxisData* axis) {
        if (axes.size() < 4) axes.push_back(axis); //? Limit to 4(could be 5) axes due to unexpected behaviour with 6 - fix later if need!
            else Serial.println("Error: Cannot add more than 4 axes!");
    }
    
    void homeGroup(){
        const uint8_t activeSwitches = getActiveSwitches(); // Get the current state of switches
        
        for (auto& axis : axes) {
            bool isCurrentlyActive = activeSwitches & (1 << axis->axis); // Current state (use axis enum val to check bit position)
            bool wasPreviouslyActive = groupPreviousSwitchStatus & (1 << axis->axis); // Previous state
            if (!axis->isHomingDone){
                homeAxis(isCurrentlyActive, wasPreviouslyActive, *axis);
            }
        }
        // Once the group is processed, update previousSwitchStatus globally
        groupPreviousSwitchStatus = activeSwitches; // Update the previous switch status for the next iteration
        // Check if all axes in the group are homed
        isGroupHomed = std::all_of(axes.begin(), axes.end(), [](AxisData* axis) {
            return axis->isHomingDone;
        });
    }
};

struct HomingManager {
    std::vector<std::unique_ptr<AxisGroup>> groups; // Vector of axis groups

    void addGroup(std::unique_ptr<AxisGroup> group) {
        if (groups.size() < 6) groups.push_back(std::move(group));
            else Serial.println("Error: Cannot add more than 6 groups!");
    }

    void executeHoming() {
        uint8_t activeSwitches = getActiveSwitches();// Get the current state of switches
        for (auto& group : groups) {
            if (!group->isGroupHomed) {
                group->homeGroup(); // Home current group
                if(!group->isGroupHomed) break; // Exit after homing one group
            } 
        }
        PREVIOUS_SWITCH_STATUS = activeSwitches;    // Update previousSwitchStatus globally after all groups processed
    }

    // void resetGroup(AxisGroup& group) {
    //     group.isGroupHomed = false;
    //     for (auto* axis : group.axes) {
    //         axis->isHomingDone = false;
    //     }
    // }  
    
    void resetAllGroups() {
        for (auto& group : groups) {
            group->isGroupHomed = false;
            for (auto* axis : group->axes) {
                axis->isHomingDone = false;
            }
        }
    }
};

// Create HomingManager
HomingManager homingManager;

void setup() { 
  Serial.begin(9600);
  while(!Serial){};
    
  // Create and add groups
  auto group1 = std::make_unique<AxisGroup>();
//   group1->addAxis(&axis1);
//   group1->addAxis(&axis2);
//   group1->addAxis(&axis3);
//   group1->addAxis(&axis4);
//   group1->addAxis(&axis5);
  group1->addAxis(&axis6);
  
  auto group2 = std::make_unique<AxisGroup>();
  group2->addAxis(&axis2);
//   group2->addAxis(&axis3);

  auto group3 = std::make_unique<AxisGroup>();
//   group3->addAxis(&axis5);
//   group3->addAxis(&axis6);
  group3->addAxis(&axis3);

  auto group4 = std::make_unique<AxisGroup>();
  group4->addAxis(&axis4);
  auto group5 = std::make_unique<AxisGroup>();
  group5->addAxis(&axis5);
  auto group6 = std::make_unique<AxisGroup>();
  group6->addAxis(&axis6);



  homingManager.addGroup(std::move(group1));
//   homingManager.addGroup(std::move(group2));
//   homingManager.addGroup(std::move(group3));
//   homingManager.addGroup(std::move(group4));
//   homingManager.addGroup(std::move(group5));
//   homingManager.addGroup(std::move(group6));




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
  
  motorJ1.setMaxSpeed(50'000); // 25
  motorJ1.setAcceleration(26'000); // 12

  motorJ2.setMaxSpeed(160'000); // 80
  motorJ2.setAcceleration(50'000); // 25

  motorJ3.setMaxSpeed(60'000);
  motorJ3.setAcceleration(30'000);

  motorJ4.setMaxSpeed(40'000); // 20
  motorJ4.setAcceleration(30'000); // 5

  motorJ5.setMaxSpeed(40'000);
  motorJ5.setAcceleration(20'000);

  motorJ6.setMaxSpeed(15'000);
  motorJ6.setAcceleration(8000);
  
  limitSwitches.init();
  // serialHandler.setCommandProcessor(&cmdProcessor);

  Serial.println("Setup done");
}


void loop() {
  // serialHandler.listenForSerial();
  // programLoader.run();

    homingManager.executeHoming();
    
}


uint8_t getActiveSwitches() {
  uint8_t activeSwitches = 0;
  for(byte i = 0; i < limitSwitchPins.size(); i++) {
    if (debounceRead(limitSwitchPins[i])) {
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


uint8_t switchStatus = 0; // Status for all 6 switches default: 0=(all inactive)
void updateSwitchStatus(Axes axis, bool active) {
    if (active) {
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
    int MOVE_AWAY_VELOCITY = axisData.MOVE_AWAY_VELOCITY;
    int MOVE_BACK_VELOCITY= axisData.MOVE_BACK_VELOCITY;
    int STANDBY_POS = axisData.STANDBY_POS;
    
    switch (homingStateJ_x) {
        case MOVE_TO_SWITCH:
            if (isCurrentlyActive && !wasPreviouslyActive) {
                Serial.println(String(axis_x) + " pressed");
                motorJ_x.emergencyStop();
                updateSwitchStatus(axis_x, true);
                homingStateJ_x = MOVE_AWAY_FROM_SWITCH;
            } else {
                motorJ_x.rotateAsync(HOMING_VELOCITY);  // Move towards the switch
            }
            break;

        case MOVE_AWAY_FROM_SWITCH:
            if (!isCurrentlyActive && wasPreviouslyActive) {
                Serial.println(String(axis_x) + " released");
                motorJ_x.emergencyStop();
                updateSwitchStatus(axis_x, false);  
                homingStateJ_x = MOVE_BACK_TO_SWITCH;   
            } else {
                    motorJ_x.rotateAsync(MOVE_AWAY_VELOCITY);   // Move away from the switch
            }
            break;

        case MOVE_BACK_TO_SWITCH:
            if (isCurrentlyActive && !wasPreviouslyActive) {
                Serial.println(String(axis_x) + " pressed again");
                motorJ_x.emergencyStop();
                updateSwitchStatus(axis_x, true);
                homingStateJ_x = SET_ZERO_POINT;
            } else {
                motorJ_x.rotateAsync(MOVE_BACK_VELOCITY);  // Move back towards the switch again
            }
            break;

        case SET_ZERO_POINT:
            static bool delayStarted = false;
            static unsigned long lastTime;

            if (!delayStarted) {
                lastTime = millis(); // Initialize the timer
                delayStarted = true;
            }

            if(Utils::nonBlockingDelay(3000, lastTime)){
                Serial.println(String(axis_x) + " set zero point");
                motorJ_x.setPosition(0); // Set the current position to zero
                homingStateJ_x = MOVE_TO_STANDBY_POS;
                delayStarted = false;
            }
            break;

        case MOVE_TO_STANDBY_POS:
            
            if (!motorJ_x.isMoving) {
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

