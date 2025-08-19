/**
 * @file AxisData.h
 * @brief Defines the AxisData structure for managing individual axis properties and states.
 * @details This structure encapsulates data related to each axis in a robotic system,
 *          including homing parameters, motor control, and current state information.
 */

#ifndef AXISDATA_H
#define AXISDATA_H

#include "HomingState.h"
#include "teensystep4.h"
#include <Arduino.h>

using namespace TS4;

/**
 * @enum Axes
 * @brief Enumeration representing the individual axes of the robotic system.
 */
enum Axes
{
    J1, ///< Represents axis J1.
    J2, ///< Represents axis J2.
    J3, ///< Represents axis J3.
    J4, ///< Represents axis J4.
    J5, ///< Represents axis J5.
    J6  ///< Represents axis J6.
};

/**
 * @struct AxisData
 * @brief Represents the data and state associated with a single axis.
 */
struct AxisData
{
    /**
     * @brief The current state of the homing process for this axis.
     */
    HomingState homingState;

    /**
     * @brief Pointer to the Stepper motor associated with this axis.
     */
    Stepper* motor;

    /**
     * @brief The identifier for the axis.
     * @details Uses the Axes enumeration for easy identification.
     */
    Axes axisId;

    /**
     * @brief Velocity used during the homing process.
     */
    int HOMING_VELOCITY;

    /**
     * @brief Velocity used when moving away from the limit switch during homing.
     */
    int MOVE_AWAY_VELOCITY;

    /**
     * @brief Velocity used when moving back toward the limit switch during fine adjustment.
     */
    int MOVE_BACK_VELOCITY;

    /**
     * @brief The home position to move the axis after homing is complete.
     */
    int HOME_POS;

    /**
     * @brief Flag indicating whether the homing process for this axis is complete.
     */
    bool isHomingDone = false;

    /**
     * @brief Tracks whether a delay has been initiated for this axis.
     * @details This flag is used to check if a delay is currently in progress,
     *          which is particularly important during homing operations.
     */
    bool delayStarted = false;

    /**
     * @brief The last time a delay was initiated for this axis.
     * @details Used to manage timing for homing operations.
     */
    unsigned long lastDelayTime = 0;
};

#endif // AXISDATA_H
