/**
 * @file MoveController.h
 * @brief Controller for synchronized multi-axis movement using Bresenham algorithm
 */

#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include "MotorConfig.h"
#include "RobotDataSender.h"
#include <Arduino.h>
#include <vector>

enum AccelStage
{
    ACCEL_RAMP_UP,
    ACCEL_CRUISE,
    ACCEL_RAMP_DOWN
};

#ifndef MAX_SPEED_M1
#define MAX_SPEED_M1 6000.0f
#endif

#ifndef MAX_SPEED_M2
#define MAX_SPEED_M2 40000.0f
#endif

#ifndef MAX_SPEED_M3
#define MAX_SPEED_M3 20000.0f
#endif

#ifndef MAX_SPEED_M4
#define MAX_SPEED_M4 30000.0f
#endif

#ifndef MAX_SPEED_M5
#define MAX_SPEED_M5 30000.0f
#endif

#ifndef MAX_SPEED_M6
#define MAX_SPEED_M6 10000.0f
#endif

#ifndef MAX_Acc
#define MAX_Acc 12000.0f
#endif

class MoveController
{
public:
    MoveController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);
    void runAllSimultaneously(float m1Pos, float m2Pos, float m3Pos, float m4Pos, float m5Pos, float m6Pos, float speedOverride = 1.0f);
    bool motorsAtTarget();
    bool isRunning() const { return _isRunning; }
    void emergencyStop();
    void reset();

private:
    void updateSpeedProfile();
    void generateAccelTable();

    std::vector<MotorConfig*>& _motorConfigs;
    RobotDataSender&           _rbtDtaSender;

    bool       _isRunning;
    AccelStage _accelStage;
    int        _masterIndex;
    long       _maxDistance;
    long       _masterStepCounter;
    bool       _initialized;

    // Bresenham algorithm state
    long          _error[6];
    long          _totalSteps[6];
    long          _stepsRemaining[6];
    bool          _directions[6];
    float         _currentSpeed;
    float         _targetSpeed;
    unsigned long _stepInterval;
    unsigned long _lastStepTime;
    int           _stepCounter;
    float         _currentAcceleration = 0.0f;

    // Legacy variables
    long   longestD2G;
    String longestMotor;
    long   m1D2G, m2D2G, m3D2G, m4D2G, m5D2G, m6D2G;
    long   m1StepDelay, m2StepDelay, m3StepDelay, m4StepDelay, m5StepDelay, m6StepDelay;
    long   stepCounter;
    long   m1StepCounter, m2StepCounter, m3StepCounter, m4StepCounter, m5StepCounter, m6StepCounter;
    long   lastD2G;
    int    motorM1Dir, motorM2Dir, motorM3Dir, motorM4Dir, motorM5Dir, motorM6Dir;

    static constexpr uint16_t minPulseWidth    = 3;
    static constexpr int      ACCEL_TABLE_SIZE = 1000;
    float                     _accelTable[ACCEL_TABLE_SIZE];

    MoveController(const MoveController&)            = delete;
    MoveController& operator=(const MoveController&) = delete;
};

inline void pulseStepPin(uint8_t pin, uint16_t pulseWidth = 3)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(pin, LOW);
}

inline void setDirectionPin(uint8_t pin, bool direction)
{
    digitalWrite(pin, direction ? HIGH : LOW);
}

#endif // MOVECONTROLLER_H