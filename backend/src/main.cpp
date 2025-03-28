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
    J1 = 0,
    J2,
    J3,
    J4,
    J5,
    J6
};

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

uint8_t getActiveSwitches();
void handleActiveSwitches();
bool debounceRead(byte pin);
void updateSwitchStatus(Axes axis, bool isActive);
bool isSwitchActive(Axes axis);


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
  
  
  motorJ1.setMaxSpeed(2000);
  motorJ1.setAcceleration(1000);

  motorJ2.setMaxSpeed(2000);
  motorJ2.setAcceleration(1000);

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
  motorJ1.rotateAsync(-200);
  motorJ2.rotateAsync(-300);
  motorJ3.rotateAsync(200);
  motorJ4.rotateAsync(200);
  motorJ5.rotateAsync(200);
  motorJ6.rotateAsync(200);
  Serial.println("Setup done");
}


void loop() {
  // serialHandler.listenForSerial();
  // programLoader.run();
  if (motorJ1.isMoving || motorJ2.isMoving)
  {
    delay(10);
  }
  
  handleActiveSwitches();
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


void handleActiveSwitches() {
    uint8_t activeSwitches = getActiveSwitches();

    for (byte i = 0; i < 8; i++) { // Iterate over all 8 bits
        if (activeSwitches & (1 << i)) { // Check if bit is set
            switch (i) {
                case 0:
                    if (!isSwitchActive(J1)) {
                        Serial.println("J1");
                        motorJ1.emergencyStop();
                        updateSwitchStatus(J1, true);
                    }
                    break;

                case 1:
                    if (!isSwitchActive(J2)) {
                        Serial.println("J2");
                        motorJ2.emergencyStop();
                        updateSwitchStatus(J2, true);
                    }
                    break;

                case 2:
                    if (!isSwitchActive(J3)) {
                        Serial.println("J3");
                        motorJ3.emergencyStop();
                        updateSwitchStatus(J3, true);
                    }
                    break;

                case 3:
                    if (!isSwitchActive(J4)) {
                        Serial.println("J4");
                        motorJ4.emergencyStop();
                        updateSwitchStatus(J4, true);
                    }
                    break;

                case 4:
                    if (!isSwitchActive(J5)) {
                        Serial.println("J5");
                        motorJ5.emergencyStop();
                        updateSwitchStatus(J5, true);
                    }
                    break;

                case 5:
                    if (!isSwitchActive(J6)) {
                        Serial.println("J6");
                        motorJ6.emergencyStop();
                        updateSwitchStatus(J6, true);
                    }
                    break;

                default:
                    Serial.println("Unknown switch detected.");
                    break;
            }
        }
    }
}


uint8_t switchStatus = 0; // Status for all 6 switches default: 0=(all inactive)
void updateSwitchStatus(Axes axis, bool active) {
    if(active) {
        switchStatus |= (1 << axis);
    } else {
        switchStatus &= ~(1 << axis);
    }
}

bool isSwitchActive(Axes axis) {
    return switchStatus & (1 << axis);
}









