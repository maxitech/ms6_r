/**
 * @file HomingState.h
 * @brief Defines the states for the homing process of a robotic system.
 * @details This enumeration represents the various steps involved in the homing routine,
 *          which is typically used to calibrate and set the zero position of the system.
 */

#ifndef HOMINGSTATE_H
#define HOMINGSTATE_H

/**
 * @enum HomingState
 * @brief Represents the sequential states of the homing process.
 */
enum HomingState
{
    MOVE_TO_SWITCH,        ///< State where the system moves towards the limit switch.
    MOVE_AWAY_FROM_SWITCH, ///< State where the system moves away from the limit switch to reset.
    MOVE_BACK_TO_SWITCH,   ///< State where the system approaches the limit switch again for fine adjustment.
    SET_ZERO_POINT,        ///< State to set the zero point for calibration.
    MOVE_TO_HOME,          ///< State where the system moves to a home position post-calibration.
    COMPLETE               ///< State indicating the homing process is finished.
};

#endif // HOMINGSTATE_H
