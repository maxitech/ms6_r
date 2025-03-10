#include <Arduino.h>
#include "SerialHandler.h"
#include "LimitSwitches.h"


// pin definitions here:
// function declarations here:
// global variables here:


void setup() {
  // setup code here:
  Serial.begin(9600);
  initLimitSwitches();
}


void loop() {
  // main code here:
  readSerialInput();
  handleNewCommand();
}


// function definitions here:








