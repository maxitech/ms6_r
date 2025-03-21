#include <Arduino.h>
#include "SerialHandler.h"
#include "CommandProcessor.h"
#include "ProgramLoader.h"
#include "LimitSwitches.h"


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

// Initialize Classes
LimitSwitches limitSwitches(ledPin, limitSwitchPins); // Manages the state of limit switches
ProgramLoader programLoader(limitSwitches);           // Controls loading programs and state management
CommandProcessor cmdProcessor(programLoader);         // Parses and processes incoming commands
SerialHandler serialHandler;                          // Handles serial communication and command routing


void setup() {
  Serial.begin(9600);
  limitSwitches.init();
  serialHandler.setCommandProcessor(&cmdProcessor);
}


void loop() {
  serialHandler.listenForSerial();
  programLoader.run();
}








