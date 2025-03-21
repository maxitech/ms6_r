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
  bool anyPressed = false;
  for(byte i = 0; i < _limitSwitches.size(); i++) {
    if(_isPressed(_limitSwitches[i])) anyPressed = true;
    digitalWrite(_ledPin, anyPressed ? HIGH : LOW);
  }
}


bool LimitSwitches::_isPressed(const byte limitSwitch) {
  return digitalRead(limitSwitch) == LOW;
}
