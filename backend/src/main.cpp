#include <Arduino.h>

const byte ledPin = 13;
const byte limitJ1 = 0;
const byte limitJ2 = 1;
const byte limitJ3 = 2;
const byte limitJ4 = 3;
const byte limitJ5 = 4;
const byte limitJ6 = 5;

const byte limitSwitches[6] = {limitJ1, limitJ2, limitJ3, limitJ4, limitJ5, limitJ6};

//function declarations here:

void initLimitSwitches();
void checkLimitSwitches();


void setup() {
  // setup code here:

  initLimitSwitches();

  Serial.begin(9600);
}

void loop() {
  // main code here:

  checkLimitSwitches();
}

// function definitions here:

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
      Serial.println(limitSwitches[i] + 1);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}