/**
 * @file Homing.h
 * @brief Provides functionality to manage the homing process for groups of axes.
 * @details The Homing class handles the homing sequence for multiple axis groups,
 *          including limit switch detection, debounce handling, and state management.
 */

#ifndef HOMING_H
#define HOMING_H

#include "AxisGroup.h"
#include <Arduino.h>
#include <memory>

/**
 * @class Homing
 * @brief Manages the homing process for robotic systems with multiple axes.
 */
class Homing
{
public:
    /**
     * @brief Constructs the Homing class.
     * @param limitSwitches An array of pin numbers for the limit switches.
     */
    explicit Homing(const std::array<byte, 6>& limitSwitches);

    /**
     * @brief Adds an axis group to the homing process.
     * @param group A unique pointer to an AxisGroup instance.
     * @note The maximum number of groups is limited to 6.
     */
    void addGroup(std::unique_ptr<AxisGroup> group);

    /**
     * @brief Executes the homing sequence for all groups.
     * @details Processes active limit switches and performs homing steps
     *          for each axis within the groups.
     */
    void executeHoming();

    /**
     * @brief Checks if all axes in all groups have completed the homing process.
     * @return True if all axes are homed, false otherwise.
     */
    bool isHomingDone() const;

    /**
     * @brief Resets the homing status of a single group.
     * @param group The AxisGroup instance to reset.
     */
    void resetGroup(AxisGroup& group);

    /**
     * @brief Resets the homing status of all groups.
     */
    void resetAllGroups();

private:
    /**
     * @brief Reads the current state of all limit switches.
     * @return A bitmask representing the active switches.
     * @internal
     */
    uint8_t _getActiveSwitches();

    /**
     * @brief Reads the state of a limit switch with debounce handling.
     * @param pin The pin number of the limit switch.
     * @return True if the switch is active, false otherwise.
     * @internal
     */
    bool _debounceRead(byte pin);

    /**
     * @brief Updates the status of a specific axis based on the limit switch state.
     * @param axis The axis to update.
     * @param active True if the limit switch is active, false otherwise.
     * @internal
     */
    void _updateSwitchStatus(Axes axis, bool active);

    /**
     * @brief Handles the homing process for an individual axis.
     * @param isCurrentlyActive The current state of the axis's limit switch.
     * @param wasPreviouslyActive The previous state of the axis's limit switch.
     * @param axisData Reference to the AxisData structure of the axis.
     * @internal
     */
    void _homeAxis(bool isCurrentlyActive, bool wasPreviouslyActive, AxisData& axisData);

    const std::array<byte, 6>               _limitSwitches;              ///< Array of pin numbers for limit switches. @internal
    std::vector<std::unique_ptr<AxisGroup>> _axisGroups;                 ///< Vector of axis groups managed by the class. @internal
    uint8_t                                 _PREVIOUS_SWITCH_STATUS = 0; ///< Previous state of all limit switches. @internal
    uint8_t                                 _SWITCH_STATUS          = 0; ///< Current state of all limit switches. @internal
};

#endif // HOMING_H
