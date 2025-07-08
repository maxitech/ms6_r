#include "LimitSwitches.h"

LimitSwitches::LimitSwitches(const byte ledPin, const std::array<byte, 6>& limitSwitches)
    : _ledPin(ledPin)
    , _limitSwitches(limitSwitches)
{
}

void LimitSwitches::init()
{
    pinMode(_ledPin, OUTPUT);
    for (byte pin : _limitSwitches)
    {
        pinMode(pin, INPUT_PULLUP);
    }
}

void LimitSwitches::check()
{
    bool                             anyPressed = false;
    byte                             pressedSwitch;
    std::array<byte, Utils::NUM_DOF> switches {0, 0, 0, 0, 0, 0};

    // Determine if switch is pressed -> if so turn on led
    for (byte i = 0; i < _limitSwitches.size(); i++)
    {
        if (_isPressed(_limitSwitches[i]))
        {
            anyPressed    = true;
            pressedSwitch = _limitSwitches[i];
            break;
        }
    }
    digitalWrite(_ledPin, anyPressed ? HIGH : LOW);

    // Determine which switch is pressed and set pressed switch in array to one
    if (anyPressed)
    {
        switches[pressedSwitch] = 1;
    }
    else
    {
        for (byte& s : switches)
        {
            s = 0;
        }
    }

    // Create and send data string
    String data = _createDtaStr(switches);
    _sendDtaStr(data);
}

bool LimitSwitches::_isPressed(const byte limitSwitch)
{
    return digitalRead(limitSwitch) == LOW;
}

byte LimitSwitches::getLedPin() const
{
    return _ledPin;
}

String LimitSwitches::_createDtaStr(std::array<byte, Utils::NUM_DOF> switches)
{
    String data = "DATA:L_SWITCH*";
    for (size_t i = 0; i < switches.size(); ++i)
    {
        data += switches[i];
        if (i != switches.size() - 1)
        {
            data += ',';
        }
    }

    return data;
}

void LimitSwitches::_sendDtaStr(String& data)
{
    static unsigned long lastSend = 0;
    if (Utils::nonBlockingDelay(100, lastSend))
    {
        Serial.println(data);
    }
}