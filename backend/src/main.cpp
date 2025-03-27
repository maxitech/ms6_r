#include <Arduino.h>
#include "SerialHandler.h"
#include "CommandProcessor.h"
#include "ProgramLoader.h"
#include "LimitSwitches.h"
#include "teensystep4.h"
#include "timers/Teensy4/TMR/TMR.h"

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


// Initialize Classes
// LimitSwitches limitSwitches(ledPin, limitSwitchPins); // Manages the state of limit switches
// ProgramLoader programLoader(limitSwitches);           // Controls loading programs and state management
// CommandProcessor cmdProcessor(programLoader);         // Parses and processes incoming commands
// SerialHandler serialHandler;                          // Handles serial communication and command routing

Stepper motorJ1(motorJ1Step, motorJ1Dir); // int stepPin, int dirPin
Stepper motorJ2(motorJ2Step, motorJ2Dir);
Stepper motorJ3(motorJ3Step, motorJ3Dir);
Stepper motorJ4(motorJ4Step, motorJ4Dir);
Stepper motorJ5(motorJ5Step, motorJ5Dir);
Stepper motorJ6(motorJ6Step, motorJ6Dir);

StepperGroup::m

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
  Stepper::
  
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
  
  // delay(2000);
  // motorJ1.moveAbs(-500);
  // delay(500);
  // motorJ1.moveAbs(500);
  // delay(500);

  // motorJ2.moveAbs(1000);
  // delay(500);
  // motorJ2.moveAbs(-1000);
  // delay(500);

  // motorJ3.moveAbs(1000);
  // delay(500);
  // motorJ3.moveAbs(-1000);
  // delay(500);

  // motorJ4.moveAbs(1000);
  // delay(500);
  // motorJ4.moveAbs(-1000);
  // delay(500);

  // motorJ5.moveAbs(1000);
  // delay(500);
  // motorJ5.moveAbs(-1000);
  // delay(500);

  // motorJ6.moveAbs(500);
  // delay(500);
  // motorJ6.moveAbs(-500);
  // delay(500);
  
  // motorJ1.moveAbsAsync(1000);
  // motorJ2.moveAbsAsync(2000);
  // motorJ3.moveAbsAsync(-1000);
  // motorJ4.moveAbsAsync(1000);
  // motorJ5.moveAbsAsync(500);
  // motorJ6.moveAbsAsync(300);
 
  
  
  // while (motorJ1.isMoving || motorJ2.isMoving || motorJ3.isMoving || motorJ4.isMoving || motorJ5.isMoving || motorJ6.isMoving) {
  //   delay(500);
  // }
  // delay(500);
  
  // motorJ1.moveAbsAsync(-1000);
  // motorJ2.moveAbsAsync(-1000);
  // motorJ3.moveAbsAsync(-1000);
  // motorJ4.moveAbsAsync(-1000);
  // motorJ5.moveAbsAsync(-500);
  // motorJ6.moveAbsAsync(-300);

  // while (motorJ1.isMoving || motorJ2.isMoving || motorJ3.isMoving || motorJ4.isMoving || motorJ5.isMoving || motorJ6.isMoving) {
  //   delay(10);
  // }
  // delay(500);


  // StepperGroup g1{motorJ1, motorJ2, motorJ3, motorJ4, motorJ5, motorJ6};
  // motorJ1.setTargetAbs(1000);
  // motorJ2.setTargetAbs(4000);
  // motorJ3.setTargetAbs(1000);
  // motorJ4.setTargetAbs(2000);
  // motorJ5.setTargetAbs(1000);
  // motorJ6.setTargetAbs(800);
  // g1.move();
  // delay(100);

  // motorJ1.setTargetAbs(-1000);
  // motorJ2.setTargetAbs(-4000);
  // motorJ3.setTargetAbs(-1000);
  // motorJ4.setTargetAbs(-2000);
  // motorJ5.setTargetAbs(-1000);
  // motorJ6.setTargetAbs(-300);
  // g1.move();
  // delay(100);
  
  // limitSwitches.init();
  // serialHandler.setCommandProcessor(&cmdProcessor);
}


void loop() {
  StepperGroup g1{motorJ1, motorJ2, motorJ3, motorJ4, motorJ5, motorJ6};
  // serialHandler.listenForSerial();
  // programLoader.run();
  // motorJ1.setTargetAbs(2000);
  // motorJ2.setTargetAbs(4000);
  // motorJ3.setTargetAbs(1000);
  // motorJ4.setTargetAbs(2000);
  // motorJ5.setTargetAbs(1500);
  // motorJ6.setTargetAbs(300);
  // g1.move();
  // delay(1000);

  // motorJ1.setTargetAbs(-2000);
  // motorJ2.setTargetAbs(-4000);
  // motorJ3.setTargetAbs(-1000);
  // motorJ4.setTargetAbs(-2000);
  // motorJ5.setTargetAbs(-1500);
  // motorJ6.setTargetAbs(-300);
  // g1.move();
  // delay(1000);
}








