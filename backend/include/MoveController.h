/**
 * @file MoveController.h
 * @brief This controller class is used to handle move related functionality.
 */

#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include "MotorConfig.h"
#include "RobotDataSender.h"
#include "Utils.h"
#include <Arduino.h>
#include <optional>

// struct JogFlags
// {
//     bool   limitReached = false; ///< Flag to check if limit is reached.
//     String blockedDir   = "";    ///< Sting of which direction is blocked -> "POS" == 1 || "NEG" == -1
//     bool   runOnce      = false; ///< Flag to execute a function once.
// };

class MoveController
{
public:
    MoveController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);

    /**
     * @brief Executes the jog command for a specific joint.
     * @param positions Reference to the passed optional positions vector.
     */
    // void runAllSimultaneously(std::optional<std::vector<int32_t>>& positions);
    void runAllSimultaneously(float m1Pos, float m2Pos, float m3Pos, float m4Pos, float m5Pos, float m6Pos);

    bool motorsAtTarget();

private:
    std::vector<MotorConfig*>& _motorConfigs; ///< Reference to a Vector of motor config pointers for the robot. Note: Setup class has full ownership of this vector. @internal
    RobotDataSender&           _rbtDtaSender; ///< Reference to the data sender class for the robot. @internal

    void coordinatedM1Move();
    void coordinatedM2Move();
    void coordinatedM3Move();
    void coordinatedM4Move();
    void coordinatedM5Move();
    void coordinatedM6Move();

    // Motor pins
    const int STP1 = _motorConfigs[0]->stepPin;
    const int DIR1 = _motorConfigs[0]->dirPin;

    const int STP2 = _motorConfigs[1]->stepPin;
    const int DIR2 = _motorConfigs[1]->dirPin;

    const int STP3 = _motorConfigs[2]->stepPin;
    const int DIR3 = _motorConfigs[2]->dirPin;

    const int STP4 = _motorConfigs[3]->stepPin;
    const int DIR4 = _motorConfigs[3]->dirPin;

    const int STP5 = _motorConfigs[4]->stepPin;
    const int DIR5 = _motorConfigs[4]->dirPin;

    const int STP6 = _motorConfigs[5]->stepPin;
    const int DIR6 = _motorConfigs[5]->dirPin;

    long MAX_SPEED_M1 = 30000;
    long MAX_SPEED_M2 = 30000;
    long MAX_SPEED_M3 = 30000;
    long MAX_SPEED_M4 = 30000;
    long MAX_SPEED_M5 = 30000;
    long MAX_SPEED_M6 = 30000;

    long MAX_Acc = 10000;

    // Variables for void runAllSimultaneously()
    float  longestD2G;
    String longestMotor;

    float m1D2G;
    float m2D2G;
    float m3D2G;
    float m4D2G;
    float m5D2G;
    float m6D2G;

    float m1StepDelay;
    float m2StepDelay;
    float m3StepDelay;
    float m4StepDelay;
    float m5StepDelay;
    float m6StepDelay;

    float stepCounter;
    float m1StepCounter;
    float m2StepCounter;
    float m3StepCounter;
    float m4StepCounter;
    float m5StepCounter;
    float m6StepCounter;

    float lastD2G;

    int motorM1Dir;
    int motorM2Dir;
    int motorM3Dir;
    int motorM4Dir;
    int motorM5Dir;
    int motorM6Dir;

    const int minPulseWidth = 20; // minimum pulse (in microseconds) width of manual steps initiated in Coordinated Move functions

    AccelStepper* motor1 = _motorConfigs[0]->motor;
    AccelStepper* motor2 = _motorConfigs[1]->motor;
    AccelStepper* motor3 = _motorConfigs[2]->motor;
    AccelStepper* motor4 = _motorConfigs[3]->motor;
    AccelStepper* motor5 = _motorConfigs[4]->motor;
    AccelStepper* motor6 = _motorConfigs[5]->motor;

    enum TestState
    {
        GO_FORWARD,
        WAIT_FORWARD,
        GO_BACKWARD,
        WAIT_BACKWARD
    };
    TestState     testState = GO_FORWARD;
    unsigned long waitStart = 0;
};

#endif // MOVECONTROLLER_h