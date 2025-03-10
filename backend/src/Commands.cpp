#include "Commands.h"
#include "SerialHandler.h"


void processPingCommand() {
  delay(500);
  bool newCommandReceived = false;

  while (!newCommandReceived) {
    Serial.println("PONG");
    delay(1000);

    if (Serial.available() > 0) {
      readSerialInput();
      newCommandReceived = true;
    }
  }
}


void processTestSwitchesCommand() {
  delay(500);
  bool newCommandReceived = false;

  while (!newCommandReceived) {
    checkLimitSwitches();

    if (Serial.available() > 0) {
      readSerialInput();
      newCommandReceived = true;
    }
  }
}
