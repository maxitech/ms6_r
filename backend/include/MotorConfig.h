/**
 * @file MotorConfig.h
 * @brief Defines the MotorConfig structure for configuring stepper motors.
 */

#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

#include "Compat.h"
#include <memory>

// using namespace TS4;

/**
 * @brief Configuration structure for a stepper motor setup.
 *
 * This struct holds all necessary parameters to describe how a motor is driven,
 * including step resolution, gearing, and microstepping settings.
 */
struct MotorConfig
{
    /**
     * @brief Arduino digital pin number for the step control.
     */
    int stepPin;

    /**
     * @brief Arduino digital pin number for the direction control.
     */
    int dirPin;

    /**
     * @brief Pointer to the Stepper motor instance.
     */
    AccelStepper* motor;

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
     * @brief Home offset in steps.
     * This is set to the defined home position and used to calculate the mathematical 0° position after homing.
     */
    int homeOffsetSteps;

    /**
     * @brief Minimum angle of the motor.
     */
    double minAngleDeg;

    /**
     * @brief Maximum angle of the motor.
     */
    double maxAngleDeg;

    /**
     * @brief Constructs a new MotorConfig object.
     * @param stepPin Arduino digital pin number for the step control.
     * @param dirPin Arduino digital pin number for the direction control.
     * @param motor Pointer to the Stepper motor instance.
     * @param homeOffset Offset in steps from limit switch(=== HOME_POS).
     * @param minAngleDeg Minimum angle limit of the motor/joint
     * @param maxAngleDeg Maximum angle limit of the motor/joint
     * @param spr Steps per revolution of the motor.
     * @param ms Microsteps per full step.
     * @param drt Number of teeth on the driving pulley (default 1.0).
     * @param dnt Number of teeth on the driven pulley (default 1.0).
     * @param gr Gearbox ratio applied to the motor (default 1.0).
     */
    MotorConfig(int stepPin, int dirPin, AccelStepper* motor, int homeOffset, float minAngleDeg, float maxAngleDeg, int spr, int ms, float drt = 1.0f, float dnt = 1.0f, float gr = 1.0f)
        : stepPin(stepPin)
        , dirPin(dirPin)
        , motor(motor)
        , stepsPerRev(spr)
        , microsteps(ms)
        , driverTeeth(drt)
        , drivenTeeth(dnt)
        , gearboxRatio(gr)
        , homeOffsetSteps(homeOffset)
        , minAngleDeg(minAngleDeg)
        , maxAngleDeg(maxAngleDeg)
    {
    }
};

#endif // MOTORCONFIG_H
