#ifndef AXISGROUP_H
#define AXISGROUP_H

#include "AxisData.h"
#include <Arduino.h>

struct AxisGroup
{
    std::vector<AxisData*> axes;
    bool                   isGroupHomed              = false;
    uint8_t                groupPreviousSwitchStatus = 0; // Track switches locally for the group

    void addAxis(AxisData* axis)
    {
        if (axes.size() < 4)
            axes.push_back(axis); //? Limit to 4(could be 5) axes due to unexpected behaviour with 6 - fix later if need!
        else
            Serial.println("Error: Cannot add more than 4 axes!");
    }

    void homeGroup(const uint8_t activeSwitches, const std::function<void(bool, bool, AxisData&)>& homeAxisCallback)
    {
        // const uint8_t activeSwitches = getActiveSwitches(); // Get the current state of switches

        for (const auto& axis : axes)
        {
            bool isCurrentlyActive   = activeSwitches & (1 << axis->axis);            // Current state (use axis enum val to check bit position)
            bool wasPreviouslyActive = groupPreviousSwitchStatus & (1 << axis->axis); // Previous state
            if (!axis->isHomingDone)
            {
                homeAxisCallback(isCurrentlyActive, wasPreviouslyActive, *axis);
            }
        }
        // Once the group is processed, update previousSwitchStatus globally
        groupPreviousSwitchStatus = activeSwitches; // Update the previous switch status for the next iteration
        // Check if all axes in the group are homed
        isGroupHomed = std::all_of(axes.begin(), axes.end(), [](AxisData* axis)
                                   { return axis->isHomingDone; });
    }
};

#endif // AXISGROUP_H