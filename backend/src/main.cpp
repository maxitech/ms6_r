#include <Arduino.h>


// pin definitions here:
const byte ledPin = 13;
const byte limitJ1 = 0;
const byte limitJ2 = 1;
const byte limitJ3 = 2;
const byte limitJ4 = 3;
const byte limitJ5 = 4;
const byte limitJ6 = 5;

const byte limitSwitches[6] = {limitJ1, limitJ2, limitJ3, limitJ4, limitJ5, limitJ6};


// function declarations here:
void initLimitSwitches();
void checkLimitSwitches();
void readSerialInput();
void handleNewCommand();
void processPingCommand();
void processTestSwitchesCommand();


// global variables here:
String command;


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


void initLimitSwitches() {
  pinMode(ledPin, OUTPUT);
  for (byte i = 0; i < 6; i++) {
    pinMode(limitSwitches[i], INPUT_PULLUP);
  }
}


void checkLimitSwitches() {
    for (byte i = 0; i < 6; i++) {
    if(digitalRead(limitSwitches[i]) == LOW) {
      digitalWrite(ledPin, HIGH);
      // Serial.println(limitSwitches[i] + 1);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}