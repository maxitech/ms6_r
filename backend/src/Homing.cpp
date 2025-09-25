#include "Homing.h"
#include "DebugLog.h"
#include "Utils.h"

#define LOG(level, msg) DebugLog::log(level, msg)

Homing::Homing(const std::array<byte, 6>& limitSwitches)
    : _limitSwitches(limitSwitches) {};

void Homing::addGroup(std::unique_ptr<AxisGroup> group)
{
    if (_axisGroups.size() < 6)
        _axisGroups.push_back(std::move(group));
    else
        LOG(LOG_ERROR, "Cannot add more than 6 groups.");
}

void Homing::executeHoming()
{
    const uint8_t activeSwitches = _getActiveSwitches(); // Get the current state of switches
    for (auto& group : _axisGroups)
    {
        if (!group->isGroupHomed)
        {
            // Pass homeAxis as a lambda
            group->homeGroup(activeSwitches, [this](bool isCurrentlyActive, bool wasPreviouslyActive, AxisData& axisData)
                             { this->_homeAxis(isCurrentlyActive, wasPreviouslyActive, axisData); });

            if (!group->isGroupHomed)
                break; // Exit after homing one group
        }
    }
    _PREVIOUS_SWITCH_STATUS = activeSwitches; // Update previousSwitchStatus globally after all groups processed
}

bool Homing::isHomingDone() const
{
    return std::all_of(_axisGroups.begin(), _axisGroups.end(),
                       [](const std::unique_ptr<AxisGroup>& group)
                       { return group->isGroupHomed; });
}

uint8_t Homing::getHomedMask() const
{
    uint8_t mask = 0;
    for (const auto& group : _axisGroups)
    {
        for (const auto& axis : group->axes)
        {
            if (axis->isHomingDone)
            {
                mask |= (1 << static_cast<int>(axis->axisId));
            }
        }
    }
    return mask;
}

void Homing::resetGroup(AxisGroup& group)
{
    group.isGroupHomed = false;
    for (auto* axis : group.axes)
    {
        axis->isHomingDone = false;
        axis->homingState  = MOVE_TO_SWITCH; // Reset homing state to initial state
    }
}

void Homing::resetAllGroups()
{
    for (auto& group : _axisGroups)
    {
        group->isGroupHomed = false;
        for (auto* axis : group->axes)
        {
            axis->isHomingDone = false;
            axis->homingState  = MOVE_TO_SWITCH; // Reset homing state to initial state
        }
    }
}

uint8_t Homing::_getActiveSwitches()
{
    uint8_t activeSwitches = 0;
    for (byte i = 0; i < _limitSwitches.size(); i++)
    {
        if (_debounceRead(_limitSwitches[i], i))
        {
            activeSwitches |= (1 << i);
        }
    }
    return activeSwitches;
}

bool Homing::_debounceRead(byte pin, int index)
{
    bool           currentState = digitalRead(pin);
    DebounceState& state        = _debounceStates[index];

    if (currentState != state.lastReadState)
    {
        state.lastChangeTime = millis(); // Sofort beim Wechsel setzen
        state.lastReadState  = currentState;
    }

    if (millis() - state.lastChangeTime > 5)
    {
        if (currentState != state.lastStableState)
        {
            state.lastStableState = currentState;
            return state.lastStableState == LOW;
        }
    }
    return state.lastStableState == LOW;
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
                       AxisData& axisData)
{
    // Extracting data from the AxisData struct
    HomingState&  homingStateJ_x     = axisData.homingState;
    AccelStepper& motorJ_x           = *(axisData.motor);
    Axes          axis_x             = axisData.axisId;
    int           HOMING_VELOCITY    = axisData.HOMING_VELOCITY;
    int           MOVE_AWAY_VELOCITY = axisData.MOVE_AWAY_VELOCITY;
    int           MOVE_BACK_VELOCITY = axisData.MOVE_BACK_VELOCITY;
    int           HOME_POS           = axisData.HOME_POS;

    switch (homingStateJ_x)
    {
    case MOVE_TO_SWITCH:
        if (isCurrentlyActive && !wasPreviouslyActive)
        {
            motorJ_x.setSpeed(0);
            _updateSwitchStatus(axis_x, true);
            homingStateJ_x = MOVE_AWAY_FROM_SWITCH;
        }
        else
        {
            // Move towards the switch
            motorJ_x.setSpeed(HOMING_VELOCITY);
            motorJ_x.runSpeed();
        }
        break;

    case MOVE_AWAY_FROM_SWITCH:
        if (!isCurrentlyActive && wasPreviouslyActive)
        {
            motorJ_x.setSpeed(0);
            _updateSwitchStatus(axis_x, false);
            homingStateJ_x = MOVE_BACK_TO_SWITCH;
        }
        else
        {
            // Move away from the switch
            motorJ_x.setSpeed(MOVE_AWAY_VELOCITY);
            motorJ_x.runSpeed();
        }
        break;

    case MOVE_BACK_TO_SWITCH:
        if (isCurrentlyActive && !wasPreviouslyActive)
        {
            motorJ_x.setSpeed(0);
            _updateSwitchStatus(axis_x, true);
            homingStateJ_x = SET_ZERO_POINT;
        }
        else
        {
            // Move back towards the switch again
            motorJ_x.setSpeed(MOVE_BACK_VELOCITY);
            motorJ_x.runSpeed();
        }
        break;

    case SET_ZERO_POINT:
        motorJ_x.setCurrentPosition(0); // Set the current position to zero

        if (!axisData.delayStarted)
        {
            axisData.lastDelayTime = millis(); // Initialize the timer
            axisData.delayStarted  = true;
        }

        if (Utils::nonBlockingDelay(100, axisData.lastDelayTime))
        {

            homingStateJ_x        = MOVE_TO_HOME;
            axisData.delayStarted = false;
        }
        break;

    case MOVE_TO_HOME:

        if (motorJ_x.currentPosition() != HOME_POS)
        {

            if (!motorJ_x.isRunning())
            {
                motorJ_x.moveTo(HOME_POS);
            }
            motorJ_x.run();
        }
        else
        {

            homingStateJ_x = COMPLETE;
        }

        if (!isCurrentlyActive && wasPreviouslyActive)
        {
            _updateSwitchStatus(axis_x, false);
        }
        break;

    case COMPLETE:
        // Serial.println(String(axis_x) + " homing complete");
        axisData.isHomingDone = true; // Set the homing done flag to true
        break;
    }
}
