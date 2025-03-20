#include <Arduino.h>
#include "SerialHandler.h"
#include "CommandProcessor.h"
#include "ProgramLoader.h"


ProgramLoader programLoader;
SerialHandler serialHandler;
CommandProcessor cmdProcessor(programLoader);


void setup() {
  // setup code here:
  Serial.begin(9600);
  serialHandler.setCommandProcessor(&cmdProcessor);
}


void loop() {
  // main code here:
  serialHandler.listenForSerial();
  programLoader.run();
}








