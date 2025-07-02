/**
 * @file Utils.h
 * @brief Provides utility functions.
 * @details This class contains general-purpose utility functions that can be used across
 *          various components of a robotic or embedded system. Currently, it focuses on
 *          facilitating a non-blocking delay mechanism.
 */

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * @class Utils
 * @brief A utility class offering static helper functions for system operations.
 */
class Utils
{
public:
    /**
     * @brief Constructs the Utils class.
     * @details Although the Utils class primarily contains static methods, the constructor
     *          is included for completeness.
     */
    Utils();

    /**
     * @brief Implements a non-blocking delay.
     * @param interval The delay interval in milliseconds.
     * @param lastUpdateTime A reference to the last recorded time. This value is updated
     *                       when the interval elapses.
     * @return True if the interval has elapsed since the last update, false otherwise.
     * @note This function relies on the `millis()` function and avoids halting the program,
     *       making it suitable for time-sensitive applications.
     */
    static bool nonBlockingDelay(unsigned long interval, unsigned long& lastUpdateTime);

    constexpr static size_t NUM_DOF = 6; ///< Number of axes/joints/motors.
};

#endif // UTILS_H
