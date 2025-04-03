#include "Homing.h"
#include "Utils.h"

Homing::Homing(const std::array<byte, 6> &limitSwitches) : _limitSwitches(limitSwitches) {};

void Homing::addGroup(std::unique_ptr<AxisGroup> group)
{
    if (_axisGroups.size() < 6)
        _axisGroups.push_back(std::move(group));
    else
        Serial.println("Error: Cannot add more than 6 groups!");
}

void Homing::executeHoming()
{
    const uint8_t activeSwitches = _getActiveSwitches(); // Get the current state of switches
    for (auto &group : _axisGroups)
    {
        if (!group->isGroupHomed)
        {
            // Pass homeAxis as a lambda
            group->homeGroup(activeSwitches, [this](bool isCurrentlyActive, bool wasPreviouslyActive, AxisData &axisData)
                             { this->_homeAxis(isCurrentlyActive, wasPreviouslyActive, axisData); });

            if (!group->isGroupHomed)
                break; // Exit after homing one group
        }
    }
    _PREVIOUS_SWITCH_STATUS = activeSwitches; // Update previousSwitchStatus globally after all groups processed
}

void Homing::resetGroup(AxisGroup &group)
{
    group.isGroupHomed = false;
    for (auto *axis : group.axes)
    {
        axis->isHomingDone = false;
    }
}

void Homing::resetAllGroups()
{
    for (auto &group : _axisGroups)
    {
        group->isGroupHomed = false;
        for (auto *axis : group->axes)
        {
            axis->isHomingDone = false;
        }
    }
}

uint8_t Homing::_getActiveSwitches()
{
    uint8_t activeSwitches = 0;
    for (byte i = 0; i < _limitSwitches.size(); i++)
    {
        if (_debounceRead(_limitSwitches[i]))
        {
            activeSwitches |= (1 << i);
        }
    }
    return activeSwitches;
}

bool Homing::_debounceRead(byte pin)
{
    //   static unsigned long lastDebounceTime[limitSwitchPins.size()] = {0};
    //   if (digitalRead(pin) == LOW) {
    //       if(Utils::nonBlockingDelay(100, lastDebounceTime[pin])) {
    //           return digitalRead(pin) == LOW;
    //       }
    //   }
    //   return false;
    // }
    if (digitalRead(pin) == LOW)
    {
        delay(5); // debounce time -> adjust if needed (5ms is a good starting
                  // point) **curr. blocking fix later if needed**
        return digitalRead(pin) == LOW;
    }
    return false;
}

void Homing::_updateSwitchStatus(Axes axis, bool active)
{
    if (active)
    {
        _SWITCH_STATUS |= (1 << axis);
    }
    else
    {
        _SWITCH_STATUS &= ~(1 << axis);
    }
}

void Homing::_homeAxis(bool isCurrentlyActive, bool wasPreviouslyActive,
                       AxisData &axisData)
{
    // Extracting data from the AxisData struct
    HomingState &homingStateJ_x = axisData.homingState;
    Stepper &motorJ_x = *(axisData.motor);
    Axes axis_x = axisData.axis;
    int HOMING_VELOCITY = axisData.HOMING_VELOCITY;
    int MOVE_AWAY_VELOCITY = axisData.MOVE_AWAY_VELOCITY;
    int MOVE_BACK_VELOCITY = axisData.MOVE_BACK_VELOCITY;
    int STANDBY_POS = axisData.STANDBY_POS;

    switch (homingStateJ_x)
    {
    case MOVE_TO_SWITCH:
        if (isCurrentlyActive && !wasPreviouslyActive)
        {
            Serial.println(String(axis_x) + " pressed");
            motorJ_x.emergencyStop();
            _updateSwitchStatus(axis_x, true);
            homingStateJ_x = MOVE_AWAY_FROM_SWITCH;
        }
        else
        {
            motorJ_x.rotateAsync(
                HOMING_VELOCITY); // Move towards the switch
        }
        break;

    case MOVE_AWAY_FROM_SWITCH:
        if (!isCurrentlyActive && wasPreviouslyActive)
        {
            Serial.println(String(axis_x) + " released");
            motorJ_x.emergencyStop();
            _updateSwitchStatus(axis_x, false);
            homingStateJ_x = MOVE_BACK_TO_SWITCH;
        }
        else
        {
            motorJ_x.rotateAsync(
                MOVE_AWAY_VELOCITY); // Move away from the switch
        }
        break;

    case MOVE_BACK_TO_SWITCH:
        if (isCurrentlyActive && !wasPreviouslyActive)
        {
            Serial.println(String(axis_x) + " pressed again");
            motorJ_x.emergencyStop();
            _updateSwitchStatus(axis_x, true);
            homingStateJ_x = SET_ZERO_POINT;
        }
        else
        {
            motorJ_x.rotateAsync(
                MOVE_BACK_VELOCITY); // Move back towards the switch again
        }
        break;

    case SET_ZERO_POINT:
        static bool delayStarted = false;
        static unsigned long lastTime;

        if (!delayStarted)
        {
            lastTime = millis(); // Initialize the timer
            delayStarted = true;
        }

        if (Utils::nonBlockingDelay(3000, lastTime))
        {
            Serial.println(String(axis_x) + " set zero point");
            motorJ_x.setPosition(0); // Set the current position to zero
            homingStateJ_x = MOVE_TO_STANDBY_POS;
            delayStarted = false;
        }
        break;

    case MOVE_TO_STANDBY_POS:

        if (!motorJ_x.isMoving)
        {
            if (motorJ_x.getPosition() != STANDBY_POS)
            {
                Serial.println(String(axis_x) +
                               " move to operating position");
                motorJ_x.moveAbsAsync(
                    STANDBY_POS); // Move to a safe position after homing
            }
            else
            {
                Serial.println(String(axis_x) +
                               " reached operating position");
                homingStateJ_x = COMPLETE;
            }
        }

        if (!isCurrentlyActive && wasPreviouslyActive)
        {
            Serial.println(String(axis_x) +
                           " released while moving to operating position");
            _updateSwitchStatus(axis_x, false);
        }
        break;

    case COMPLETE:
        // Serial.println(String(axis_x) + " homing complete");
        axisData.isHomingDone = true; // Set the homing done flag to true
        break;
    }
}
