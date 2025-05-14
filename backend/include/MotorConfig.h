/**
 * @file MotorConfig.h
 * @brief Defines the MotorConfig structure for configuring stepper motors.
 */

#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

#include "Compat.h"
#include <memory>

using namespace TS4;

/**
 * @brief Configuration structure for a stepper motor setup.
 *
 * This struct holds all necessary parameters to describe how a motor is driven,
 * including step resolution, gearing, and microstepping settings.
 */
struct MotorConfig
{
    /**
     * @brief Pointer to the Stepper motor instance.
     */
    Stepper* motor;

    /**
     * @brief Number of full steps the motor takes per revolution.
     */
    int stepsPerRev;

    /**
     * @brief Number of microsteps per full step.
     */
    int microsteps;

    /**
     * @brief Number of teeth on the driving pulley or gear.
     * Default is 1.0.
     */
    float driverTeeth;

    /**
     * @brief Number of teeth on the driven pulley or gear.
     * Default is 1.0.
     */
    float drivenTeeth;

    /**
     * @brief Gearbox ratio applied to the motor.
     * Default is 1.0 (i.e., no gearbox).
     */
    float gearboxRatio;

    /**
     * @brief Constructs a new MotorConfig object.
     *
     * @param motor Pointer to the Stepper motor instance.
     * @param spr Steps per revolution of the motor.
     * @param ms Microsteps per full step.
     * @param drt Number of teeth on the driving pulley (default 1.0).
     * @param dnt Number of teeth on the driven pulley (default 1.0).
     * @param gr Gearbox ratio applied to the motor (default 1.0).
     */
    MotorConfig(Stepper* motor, int spr, int ms, float drt = 1.0f, float dnt = 1.0f, float gr = 1.0f)
        : motor(motor)
        , stepsPerRev(spr)
        , microsteps(ms)
        , driverTeeth(drt)
        , drivenTeeth(dnt)
        , gearboxRatio(gr)
    {
    }
};

#endif // MOTORCONFIG_H
