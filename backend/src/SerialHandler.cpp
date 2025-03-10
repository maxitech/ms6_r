#include "SerialHandler.h"

// global variable here:
String command;


void readSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();
    Serial.println("Loaded Program - " + input);
    command = input;
  }
}


void handleNewCommand() {
  if (command == "PING") {
    processPingCommand();
  } else if (command == "TEST_SWITCHES") {
    processTestSwitchesCommand();
  }
}