#pragma once

#include "Arduino.h"
#include "MotorConfig.h"
#include <cstdint>
#include <vector>

class TrajectoryExecutor
{
public:
    TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs);
    ~TrajectoryExecutor();

    // Upload-Protokoll (BLEIBT IDENTISCH!)
    void startUpload(uint32_t numPoints, uint32_t totalTimeMs);
    void addPoint(uint32_t timeMs, const int32_t steps[6]);
    void endUpload();
    void execute();
    void cancel();

    bool isRunning() const { return _isRunning; }
    bool isUploading() const { return _isUploading; }

    // WICHTIG: Update-Methode (wird in loop() aufgerufen)
    void update();

private:
    struct TrajectoryPoint
    {
        uint32_t timeMs;
        int32_t  steps[6];
    };

    std::vector<MotorConfig*>&   _motorConfigs;
    std::vector<TrajectoryPoint> _trajectory;

    bool _isUploading      = false;
    bool _isRunning        = false;
    bool _trajectoryLoaded = false;

    // Zeitsteuerung
    uint32_t _trajectoryStartTime = 0;
    uint32_t _totalTimeMs         = 0;

    // Aktueller Index in der Trajektorie
    size_t _currentIndex = 0;

    // Letzte Aktualisierungszeit
    uint32_t _lastUpdateTime = 0;

    // Aktuelle Geschwindigkeiten für runSpeed()
    int32_t _currentSpeeds[6] = {0, 0, 0, 0, 0, 0};

    // Aktuelle Zielpositionen
    int32_t _currentTargets[6] = {0, 0, 0, 0, 0, 0};

    // Startpositionen (für Interpolation)
    int32_t _startPositions[6] = {0, 0, 0, 0, 0, 0};

    // Private Methoden
    void _calculateTargetForCurrentTime();
    void _updateMotorSpeeds();
    void _sendReachedSignal();
};

//!!!!
// #pragma once

// #include "Arduino.h"
// #include "MotorConfig.h"
// #include <cstdint>
// #include <vector>

// class TrajectoryExecutor
// {
// public:
//     TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs);
//     ~TrajectoryExecutor();

//     void begin();

//     // Upload-Protokoll (ZEITSTEMPEL in Millisekunden!)
//     void startUpload(uint32_t numPoints, uint32_t totalTimeMs);
//     void addPoint(uint32_t timeMs, const int32_t steps[6]);
//     void endUpload();
//     void execute();
//     void cancel();
//     void emergencyStop();

//     bool  isRunning() const { return _isRunning; }
//     bool  isUploading() const { return _isUploading; }
//     float getProgress() const;

//     void update();

//     const int32_t* getCurrentSteps() const { return _currentSteps; }

// private:
//     struct TrajectoryPoint
//     {
//         uint32_t timeMs;
//         int32_t  steps[6];
//     };

//     std::vector<MotorConfig*>&   _motorConfigs;
//     std::vector<TrajectoryPoint> _trajectory;

//     bool _isUploading      = false;
//     bool _isRunning        = false;
//     bool _trajectoryLoaded = false;

//     // Zeitbasis
//     uint32_t _trajectoryStartTime = 0;
//     uint32_t _totalTimeMs         = 0;

//     // Aktuelle Position
//     int32_t _currentSteps[6];
//     int32_t _targetSteps[6];

//     // Aktuelles Segment
//     uint32_t _currentSegmentIdx = 0;
//     int32_t  _segmentStartSteps[6];
//     int32_t  _segmentEndSteps[6];
//     uint32_t _segmentStartTime = 0;
//     uint32_t _segmentEndTime   = 0;

//     // Interpolation
//     int32_t _interpolatedTarget[6];
//     int32_t _bresenhamDelta[6];
//     int32_t _bresenhamError[6];
//     int32_t _bresenhamMaxDelta = 0;

//     // Timer mit 1kHz (1ms) für gute Balance zwischen Speed und Smoothness
//     IntervalTimer             _stepTimer;
//     static constexpr uint32_t STEP_TIMER_PERIOD_US = 1000; // 1ms = 1kHz

//     // Private Methoden
//     void        _stepTimerISR();
//     static void _staticStepTimerISR();
//     void        _updateInterpolatedTarget();
//     void        _updateBresenhamToInterpolated();
//     void        _executeBresenhamStep();
//     void        _pulseStep(uint8_t motorIdx);
//     void        _setDirection(uint8_t motorIdx, bool direction);

//     static TrajectoryExecutor* _instance;
// };
//!!!
// #pragma once

// #include "Arduino.h"
// #include "MotorConfig.h"
// #include <cstdint>
// #include <vector>

// class TrajectoryExecutor
// {
// public:
//     TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs);
//     ~TrajectoryExecutor();

//     void begin();

//     // Protokoll-Handler
//     void startUpload(uint32_t numPoints, uint32_t totalTimeMs);
//     void addPoint(uint32_t timeMs, const int32_t steps[6]);
//     void endUpload();
//     void execute();
//     void cancel();
//     void emergencyStop();

//     bool  isRunning() const { return _isRunning; }
//     bool  isUploading() const { return _isUploading; }
//     float getProgress() const;

//     void update();

//     const int32_t* getCurrentSteps() const { return _currentSteps; }

// private:
//     struct TrajectoryPoint
//     {
//         uint32_t timeMs;
//         int32_t  steps[6];
//     };

//     std::vector<MotorConfig*>&   _motorConfigs;
//     std::vector<TrajectoryPoint> _trajectory;

//     bool _isUploading      = false;
//     bool _isRunning        = false;
//     bool _trajectoryLoaded = false;

//     // Ausführungsstatus
//     uint32_t _trajectoryStartTime = 0;
//     uint32_t _totalTimeMs         = 0;

//     // Positions-Tracking
//     int32_t _currentSteps[6]; // Aktuelle ISR-Position
//     int32_t _targetSteps[6];  // Finales Ziel (letzter Punkt)

//     // Segment-basierte Interpolation
//     uint32_t _currentSegmentIdx = 0;
//     int32_t  _segmentStartSteps[6];
//     int32_t  _segmentEndSteps[6];
//     uint32_t _segmentStartTime = 0;
//     uint32_t _segmentEndTime   = 0;

//     // Bresenham für aktuelles interpoliertes Ziel
//     int32_t _interpolatedTarget[6];
//     int32_t _bresenhamDelta[6];
//     int32_t _bresenhamError[6];
//     int32_t _bresenhamMaxDelta = 0;

//     // Hardware-Timer
//     IntervalTimer             _stepTimer;
//     static constexpr uint32_t STEP_TIMER_PERIOD_US = 2000; // 500Hz = 2ms

//     // Private Methoden
//     void        _stepTimerISR();
//     static void _staticStepTimerISR();
//     void        _updateInterpolatedTarget();
//     void        _updateBresenhamToInterpolated();
//     void        _executeBresenhamStep();
//     void        _pulseStep(uint8_t motorIdx);
//     void        _setDirection(uint8_t motorIdx, bool direction);

//     static TrajectoryExecutor* _instance;
// };

//!!!!
// #pragma once

// #include "Arduino.h"
// #include "MotorConfig.h"
// #include <cstdint>
// #include <vector>

// class TrajectoryExecutor
// {
// public:
//     TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs);
//     ~TrajectoryExecutor();

//     void begin();

//     // Protokoll-Handler
//     void startUpload(uint32_t numPoints, uint32_t totalTimeMs);
//     void addPoint(uint32_t timeMs, const int32_t steps[6]);
//     void endUpload();
//     void execute();
//     void cancel();
//     void emergencyStop();

//     bool  isRunning() const { return _isRunning; }
//     bool  isUploading() const { return _isUploading; }
//     float getProgress() const;

//     // Für bestehende Integration
//     void update(); // In loop() aufrufen

//     // Position sync
//     const int32_t* getCurrentSteps() const { return _currentSteps; }

// private:
//     struct TrajectoryPoint
//     {
//         uint32_t timeMs;
//         int32_t  steps[6];
//     };

//     std::vector<MotorConfig*>&   _motorConfigs;
//     std::vector<TrajectoryPoint> _trajectory;

//     bool _isUploading      = false;
//     bool _isRunning        = false;
//     bool _trajectoryLoaded = false;

//     // Ausführungsstatus
//     uint32_t _trajectoryStartTime = 0;
//     uint32_t _currentPointIdx     = 0;
//     uint32_t _totalTimeMs         = 0;

//     // Zeitgesteuerte Ausführung
//     int32_t _currentSteps[6]; // Aktuelle Position der Motoren
//     int32_t _targetSteps[6];  // Interpolierte Zielposition für aktuelle Zeit
//     int32_t _error[6];        // Bresenham-Fehlerterme
//     int32_t _delta[6];        // Delta für aktuelles Segment
//     int32_t _maxDelta;        // Maximales Delta

//     // Hardware-Timer
//     IntervalTimer             _stepTimer;
//     static constexpr uint32_t STEP_TIMER_PERIOD_US = 10000; // 100Hz = 10ms für Interpolation

//     // Private Methoden
//     void        _stepTimerISR();
//     static void _staticStepTimerISR();
//     void        _interpolateTrajectory(uint32_t currentTimeMs);
//     void        _doBresenhamStepToTarget();
//     void        _pulseStep(uint8_t motorIdx);
//     void        _setDirection(uint8_t motorIdx, bool direction);
//     void        _updateBresenhamParameters();

//     static TrajectoryExecutor* _instance;
// };

// #pragma once

// #include "Arduino.h"
// #include "MotorConfig.h"
// #include <cstdint>
// #include <vector>

// class TrajectoryExecutor
// {
// public:
//     TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs);
//     ~TrajectoryExecutor();

//     void begin();

//     // Protokoll-Handler
//     void startUpload(uint32_t numPoints, uint32_t totalTimeMs);
//     void addPoint(uint32_t timeMs, const int32_t steps[6]);
//     void endUpload();
//     void execute();
//     void cancel();
//     void emergencyStop();

//     bool  isRunning() const { return _isRunning; }
//     bool  isUploading() const { return _isUploading; }
//     float getProgress() const;

//     // Für bestehende Integration
//     void update(); // In loop() aufrufen

//     // Position sync
//     const int32_t* getCurrentSteps() const { return _currentSteps; }

// private:
//     struct TrajectoryPoint
//     {
//         uint32_t timeMs;
//         int32_t  steps[6];
//     };

//     std::vector<MotorConfig*>&   _motorConfigs;
//     std::vector<TrajectoryPoint> _trajectory;

//     bool _isUploading      = false;
//     bool _isRunning        = false;
//     bool _trajectoryLoaded = false;

//     // Ausführungsstatus
//     uint32_t _trajectoryStartTime = 0;
//     uint32_t _currentPointIdx     = 0;
//     uint32_t _totalTimeMs         = 0;

//     // Bresenham State für aktuelles Segment
//     int32_t  _currentSteps[6];
//     int32_t  _segmentStartSteps[6];
//     int32_t  _segmentTargetSteps[6];
//     int32_t  _segmentDelta[6];
//     int32_t  _error[6];
//     int32_t  _maxDelta;
//     uint32_t _segmentTotalSteps = 0; // Anzahl ISR-Aufrufe für aktuelles Segment

//     // Hardware-Timer
//     IntervalTimer             _stepTimer;
//     static constexpr uint32_t STEP_TIMER_PERIOD_US = 1000; // 1kHz

//     // Private Methoden
//     void        _stepTimerISR();
//     static void _staticStepTimerISR();
//     void        _updateSegment();
//     void        _doBresenhamStep();
//     void        _pulseStep(uint8_t motorIdx);
//     void        _setDirection(uint8_t motorIdx, bool direction);

//     static TrajectoryExecutor* _instance;
// };