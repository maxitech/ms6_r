/**
 * @file LimitSwitches.h
 * @brief Declares the LimitSwitches class for handling limit switches in a robotic system.
 * @details This class manages a set of limit switches, providing initialization and monitoring
 *          functionalities. It is designed to work with an LED indicator to signal the status
 *          of the switches.
 */

#ifndef LIMITSWITCHES_H
#define LIMITSWITCHES_H

#include <Arduino.h>

/**
 * @class LimitSwitches
 * @brief Handles limit switch monitoring and status indication.
 */
class LimitSwitches
{
public:
    /**
     * @brief Constructs the LimitSwitches class.
     * @param ledPin The pin number of the LED to indicate the status of the limit switches.
     * @param limitSwitches An array of pin numbers connected to the limit switches.
     */
    LimitSwitches(const byte ledPin, const std::array<byte, 6>& limitSwitches);

    /**
     * @brief Initializes the limit switches and the LED pin.
     */
    void init();

    /**
     * @brief Checks the status of the limit switches and updates the LED accordingly.
     */
    void check();

    byte getLedPin() const; ///< Returns the pin number of the LED.

private:
    /**
     * @brief Checks if a specific limit switch is pressed.
     * @param limitSwitch The pin number of the limit switch to check.
     * @return True if the switch is pressed, false otherwise.
     * @internal
     */
    bool _isPressed(const byte limitSwitch);

    const byte                _ledPin;        ///< The pin number of the LED. @internal
    const std::array<byte, 6> _limitSwitches; ///< Array holding pin numbers for the limit switches. @internal
};

#endif // LIMITSWITCHES_H
