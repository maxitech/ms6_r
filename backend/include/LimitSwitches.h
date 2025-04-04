#ifndef LIMITSWITCHES_H
#define LIMITSWITCHES_H

#include <Arduino.h>

class LimitSwitches
{
public:
    LimitSwitches(const byte ledPin, const std::array<byte, 6>& limitSwitches);
    void init();
    void check();

private:
    bool _isPressed(const byte limitSwitch);

    const byte                _ledPin;
    const std::array<byte, 6> _limitSwitches;
};

#endif // LIMITSWITCHES_H