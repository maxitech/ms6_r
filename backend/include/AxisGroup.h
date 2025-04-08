/**
 * @file AxisGroup.h
 * @brief Manages a group of axes for coordinated homing operations.
 * @details This structure represents a group of axes and provides functionality
 *          to add axes and handle the homing process for the entire group.
 */

#ifndef AXISGROUP_H
#define AXISGROUP_H

#include "AxisData.h"
#include <Arduino.h>

/**
 * @struct AxisGroup
 * @brief Represents a group of axes, enabling collective management and homing.
 */
struct AxisGroup
{
    /**
     * @brief A vector of pointers to AxisData objects representing the axes in the group.
     */
    std::vector<AxisData*> axes;

    /**
     * @brief Indicates whether the entire group has completed the homing process.
     */
    bool isGroupHomed = false;

    /**
     * @brief Tracks the previous switch status for all axes in the group.
     * @details Used to monitor and compare the current and past states of the limit switches.
     */
    uint8_t groupPreviousSwitchStatus = 0;

    /**
     * @brief Adds an axis to the group.
     * @param axis Pointer to an AxisData object representing the axis to add.
     * @note The number of axes is limited to 4 (potentially extendable to 5) due to
     *       unexpected behavior with 6 axes.
     */
    void addAxis(AxisData* axis)
    {
        if (axes.size() < 4)
            axes.push_back(axis); //? Limit to 4 (could be 5) axes due to unexpected behavior with 6
        else
            Serial.println("Error: Cannot add more than 4 axes!");
    }

    /**
     * @brief Handles the homing process for all axes in the group.
     * @param activeSwitches A bitmask representing the currently active limit switches.
     * @param homeAxisCallback A callback function to process the homing of each individual axis.
     * @details The function iterates through all axes in the group, determines their current
     *          and previous switch states, and invokes the callback function for each axis
     *          that has not yet completed its homing process. It also updates the switch status
     *          and checks if the entire group is homed.
     */
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
