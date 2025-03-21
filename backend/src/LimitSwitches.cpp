#include "LimitSwitches.h"


LimitSwitches::LimitSwitches(const byte ledPin, const std::array<byte, 6>& limitSwitches) 
                            : _ledPin(ledPin),  _limitSwitches(limitSwitches) {}


void LimitSwitches::init() {
  pinMode(_ledPin, OUTPUT);
  for (byte pin : _limitSwitches) {
    pinMode(pin, INPUT_PULLUP);
  }
}


void LimitSwitches::check() {
    for (byte limitSwitch : _limitSwitches) {
    if(_isPressed(limitSwitch)) {
      digitalWrite(_ledPin, HIGH);
    } else {
      digitalWrite(_ledPin, LOW);
    }
  }
}


bool LimitSwitches::_isPressed(const byte limitSwitch) {
  return digitalRead(limitSwitch) == LOW;
}
