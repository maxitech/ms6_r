#include <Arduino.h>
#include "SerialHandler.h"
#include "CommandProcessor.h"


SerialHandler serialHandler;
CommandProcessor cmdProcessor;


void setup() {
  // setup code here:
  Serial.begin(9600);
  serialHandler.setCommandProcessor(&cmdProcessor);
}


void loop() {
  // main code here:
  serialHandler.listenForSerial();
}








