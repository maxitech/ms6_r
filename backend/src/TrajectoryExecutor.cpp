#include "TrajectoryExecutor.h"
#include "ComProtocol.h"
#include "Utils.h"
#include <cstring>

using namespace CommunicationProtocoll;

TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
    : _motorConfigs(motorConfigs)
{
    memset(_currentSpeeds, 0, sizeof(_currentSpeeds));
    memset(_currentTargets, 0, sizeof(_currentTargets));
    memset(_startPositions, 0, sizeof(_startPositions));
}

TrajectoryExecutor::~TrajectoryExecutor()
{
    // Kein Timer, kein ISR!
}

// Upload-Funktionen bleiben 1:1 identisch!
void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
{
    _trajectory.clear();
    _trajectory.reserve(numPoints);
    _isUploading = true;
    _totalTimeMs = totalTimeMs;
}

void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
{
    if (!_isUploading)
        return;

    TrajectoryPoint point;
    point.timeMs = timeMs;
    memcpy(point.steps, steps, 6 * sizeof(int32_t));
    _trajectory.push_back(point);
}

void TrajectoryExecutor::endUpload()
{
    _isUploading      = false;
    _trajectoryLoaded = true;
}

// WICHTIG: Einfache execute() wie PAROL6
void TrajectoryExecutor::execute()
{
    if (!_trajectoryLoaded || _trajectory.empty())
    {
        return;
    }

    // 1. Startpositionen holen (von AccelStepper)
    for (size_t i = 0; i < _motorConfigs.size() && i < 6; i++)
    {
        if (_motorConfigs[i]->motor)
        {
            _startPositions[i] = _motorConfigs[i]->motor->currentPosition();
        }
    }

    // 2. Initialisieren
    _currentIndex        = 0;
    _trajectoryStartTime = millis();
    _lastUpdateTime      = millis();
    _isRunning           = true;

    // 3. Ersten Zielpunkt setzen
    if (!_trajectory.empty())
    {
        memcpy(_currentTargets, _trajectory[0].steps, sizeof(_currentTargets));
    }
}

// WICHTIG: Diese Methode wird in loop() aufgerufen (wie PAROL6)
void TrajectoryExecutor::update()
{
    if (!_isRunning)
        return;

    uint32_t currentTime = millis();

    // Zu oft updaten? Max. alle 10ms (wie PAROL6)
    if (currentTime - _lastUpdateTime < 10)
    {
        // Trotzdem runSpeed() aufrufen! (WICHTIG)
        for (size_t i = 0; i < _motorConfigs.size() && i < 6; i++)
        {
            if (_motorConfigs[i]->motor)
            {
                _motorConfigs[i]->motor->setSpeed(_currentSpeeds[i]);
                _motorConfigs[i]->motor->runSpeed();
            }
        }
        return;
    }

    _lastUpdateTime = currentTime;

    // Verstrichene Zeit seit Start
    uint32_t elapsedTime = currentTime - _trajectoryStartTime;

    // Ende erreicht?
    if (elapsedTime >= _totalTimeMs)
    {
        _isRunning = false;

        // Auf finale Position setzen
        if (!_trajectory.empty())
        {
            const auto& lastPoint = _trajectory.back();

            for (size_t i = 0; i < _motorConfigs.size() && i < 6; i++)
            {
                if (_motorConfigs[i]->motor)
                {
                    // Position synchronisieren
                    _motorConfigs[i]->motor->setCurrentPosition(lastPoint.steps[i]);
                    _motorConfigs[i]->motor->setSpeed(0);
                    _motorConfigs[i]->motor->runSpeed();
                }
            }
        }

        _sendReachedSignal();
        return;
    }

    // Aktuelles Ziel für diese Zeit berechnen
    _calculateTargetForCurrentTime();

    // Geschwindigkeiten für Motoren berechnen
    _updateMotorSpeeds();

    // Motoren updaten (wie PAROL6 Befehl 156)
    for (size_t i = 0; i < _motorConfigs.size() && i < 6; i++)
    {
        if (_motorConfigs[i]->motor)
        {
            // Setze Geschwindigkeit
            _motorConfigs[i]->motor->setSpeed(_currentSpeeds[i]);
            // Fahre mit dieser Geschwindigkeit
            _motorConfigs[i]->motor->runSpeed();
        }
    }
}

void TrajectoryExecutor::_calculateTargetForCurrentTime()
{
    uint32_t elapsedTime = millis() - _trajectoryStartTime;

    // Am Ende? Nimm letzten Punkt
    if (elapsedTime >= _totalTimeMs || _trajectory.empty())
    {
        if (!_trajectory.empty())
        {
            memcpy(_currentTargets, _trajectory.back().steps, sizeof(_currentTargets));
        }
        return;
    }

    // Finde richtiges Segment
    size_t segmentIndex = 0;
    for (; segmentIndex < _trajectory.size() - 1; segmentIndex++)
    {
        if (elapsedTime < _trajectory[segmentIndex + 1].timeMs)
        {
            break;
        }
    }

    // Interpolation zwischen zwei Punkten
    if (segmentIndex == 0)
    {
        // Erster Segment: Von Startposition zu erstem Punkt
        uint32_t segmentEndTime = _trajectory[0].timeMs;
        if (segmentEndTime == 0)
            segmentEndTime = 1;

        float t = (float)elapsedTime / (float)segmentEndTime;
        t       = constrain(t, 0.0f, 1.0f);

        for (int i = 0; i < 6; i++)
        {
            _currentTargets[i] = _startPositions[i] +
                                 (int32_t)((_trajectory[0].steps[i] - _startPositions[i]) * t);
        }
    }
    else if (segmentIndex < _trajectory.size())
    {
        // Zwischen zwei Punkten
        uint32_t segmentStartTime = _trajectory[segmentIndex - 1].timeMs;
        uint32_t segmentEndTime   = _trajectory[segmentIndex].timeMs;
        uint32_t segmentDuration  = segmentEndTime - segmentStartTime;

        if (segmentDuration > 0)
        {
            float t = (float)(elapsedTime - segmentStartTime) / (float)segmentDuration;
            t       = constrain(t, 0.0f, 1.0f);

            for (int i = 0; i < 6; i++)
            {
                int32_t startVal   = _trajectory[segmentIndex - 1].steps[i];
                int32_t endVal     = _trajectory[segmentIndex].steps[i];
                _currentTargets[i] = startVal + (int32_t)((endVal - startVal) * t);
            }
        }
        else
        {
            // Keine Dauer - direkt nehmen
            memcpy(_currentTargets, _trajectory[segmentIndex].steps, sizeof(_currentTargets));
        }
    }
}

// WICHTIG: Wie PAROL6 - Geschwindigkeit = (Ziel - Aktuell) / Zeit
void TrajectoryExecutor::_updateMotorSpeeds()
{
    for (size_t i = 0; i < _motorConfigs.size() && i < 6; i++)
    {
        if (_motorConfigs[i]->motor)
        {
            // Aktuelle Position
            int32_t currentPos = _motorConfigs[i]->motor->currentPosition();

            // Berechne benötigte Geschwindigkeit (Schritte/Sekunde)
            // Annahme: Nächster Update in ~10ms = 0.01s
            int32_t diff  = _currentTargets[i] - currentPos;
            int32_t speed = diff * 100; // diff / 0.01 = diff * 100

            // Auf Maximalgeschwindigkeit begrenzen
            int32_t maxSpeed = 5000; // Passe das an deine Motoren an!
            speed            = constrain(speed, -maxSpeed, maxSpeed);

            _currentSpeeds[i] = speed;
        }
    }
}

void TrajectoryExecutor::_sendReachedSignal()
{
    Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
}

void TrajectoryExecutor::cancel()
{
    _isRunning = false;

    // Motoren stoppen
    for (size_t i = 0; i < _motorConfigs.size(); i++)
    {
        if (_motorConfigs[i]->motor)
        {
            _motorConfigs[i]->motor->setSpeed(0);
        }
    }
}

//!!!

// #include "TrajectoryExecutor.h"
// #include "ComProtocol.h"
// #include "Utils.h"
// #include <cstring>

// using namespace CommunicationProtocoll;

// TrajectoryExecutor* TrajectoryExecutor::_instance = nullptr;

// TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
//     : _motorConfigs(motorConfigs)
// {
//     _instance = this;
//     memset(_currentSteps, 0, sizeof(_currentSteps));
//     memset(_targetSteps, 0, sizeof(_targetSteps));
//     memset(_interpolatedTarget, 0, sizeof(_interpolatedTarget));
//     memset(_bresenhamError, 0, sizeof(_bresenhamError));
//     memset(_bresenhamDelta, 0, sizeof(_bresenhamDelta));
// }

// TrajectoryExecutor::~TrajectoryExecutor()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
// }

// void TrajectoryExecutor::begin()
// {
//     Serial.println("TrajectoryExecutor initialized");
//     Serial.printf("Timer frequency: %d Hz (%d us period)\n",
//                   1000000 / STEP_TIMER_PERIOD_US, STEP_TIMER_PERIOD_US);
// }

// void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
// {
//     _trajectory.clear();
//     _trajectory.reserve(numPoints);
//     _isUploading = true;
//     _totalTimeMs = totalTimeMs;

//     Serial.printf("=== TRAJECTORY UPLOAD START ===\n");
//     Serial.printf("Points: %d, Duration: %d ms\n", numPoints, totalTimeMs);
// }

// void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
// {
//     if (!_isUploading)
//         return;

//     TrajectoryPoint point;
//     point.timeMs = timeMs;
//     memcpy(point.steps, steps, 6 * sizeof(int32_t));

//     _trajectory.push_back(point);
// }

// void TrajectoryExecutor::endUpload()
// {
//     _isUploading      = false;
//     _trajectoryLoaded = true;

//     Serial.printf("=== TRAJECTORY UPLOAD COMPLETE ===\n");
//     Serial.printf("Loaded %d points\n", _trajectory.size());

//     if (!_trajectory.empty())
//     {
//         const auto& first = _trajectory.front();
//         const auto& last  = _trajectory.back();

//         Serial.printf("First point: t=%dms, steps=[%d,%d,%d,%d,%d,%d]\n",
//                       first.timeMs, first.steps[0], first.steps[1], first.steps[2],
//                       first.steps[3], first.steps[4], first.steps[5]);
//         Serial.printf("Last point: t=%dms, steps=[%d,%d,%d,%d,%d,%d]\n",
//                       last.timeMs, last.steps[0], last.steps[1], last.steps[2],
//                       last.steps[3], last.steps[4], last.steps[5]);
//     }
// }

// void TrajectoryExecutor::execute()
// {
//     if (!_trajectoryLoaded || _trajectory.empty())
//     {
//         Serial.println("ERROR: Cannot execute - No trajectory loaded!");
//         return;
//     }

//     // Timer stoppen falls läuft
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }

//     Serial.println("\n=== TRAJECTORY EXECUTION START ===");

//     // WICHTIG: Aktuelle physische Position aus AccelStepper holen
//     for (size_t i = 0; i < 6; i++)
//     {
//         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//         {
//             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
//         }
//     }

//     Serial.printf("Start position: [%d,%d,%d,%d,%d,%d]\n",
//                   _currentSteps[0], _currentSteps[1], _currentSteps[2],
//                   _currentSteps[3], _currentSteps[4], _currentSteps[5]);

//     // Finales Ziel (letzter Trajektorienpunkt)
//     memcpy(_targetSteps, _trajectory.back().steps, sizeof(_targetSteps));

//     Serial.printf("Final target: [%d,%d,%d,%d,%d,%d]\n",
//                   _targetSteps[0], _targetSteps[1], _targetSteps[2],
//                   _targetSteps[3], _targetSteps[4], _targetSteps[5]);

//     // Erstes Segment initialisieren: Von aktueller Position zum ersten Punkt
//     _currentSegmentIdx = 0;
//     _segmentStartTime  = 0;
//     _segmentEndTime    = _trajectory[0].timeMs;

//     // Segment-Start = AKTUELLE Position (wo Robot jetzt ist)
//     memcpy(_segmentStartSteps, _currentSteps, sizeof(_segmentStartSteps));

//     // Segment-Ende = erster Trajektorienpunkt
//     memcpy(_segmentEndSteps, _trajectory[0].steps, sizeof(_segmentEndSteps));

//     Serial.printf("First segment: t=0ms to t=%dms\n", _segmentEndTime);
//     Serial.printf("  From: [%d,%d,...]\n", _segmentStartSteps[0], _segmentStartSteps[1]);
//     Serial.printf("  To:   [%d,%d,...]\n", _segmentEndSteps[0], _segmentEndSteps[1]);

//     // Interpoliertes Ziel initial = aktuelle Position
//     memcpy(_interpolatedTarget, _currentSteps, sizeof(_interpolatedTarget));

//     // Bresenham initialisieren
//     _updateBresenhamToInterpolated();

//     // Ausführung starten - WICHTIG: micros() statt millis()!
//     _trajectoryStartTime = micros() / 1000; // Convert to milliseconds
//     _isRunning           = true;

//     Serial.printf("Duration: %d ms (%.2f seconds)\n", _totalTimeMs, _totalTimeMs / 1000.0f);
//     Serial.println("=== ISR STARTING ===\n");

//     // Timer starten
//     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);
// }

// void TrajectoryExecutor::_stepTimerISR()
// {
//     if (!_isRunning || _trajectory.empty())
//         return;

//     // 1. Aktuelle Zeit seit Start - WICHTIG: micros() statt millis()!
//     uint32_t elapsedTime = (micros() / 1000) - _trajectoryStartTime;

//     // 2. Prüfen ob wir zum nächsten Segment wechseln müssen
//     if (_currentSegmentIdx < _trajectory.size() - 1)
//     {
//         uint32_t nextSegmentTime = _trajectory[_currentSegmentIdx + 1].timeMs;

//         if (elapsedTime >= nextSegmentTime)
//         {
//             // Nächstes Segment
//             _currentSegmentIdx++;

//             // Segment-Grenzen aktualisieren
//             _segmentStartTime = _trajectory[_currentSegmentIdx].timeMs;

//             // Start = aktueller Trajektorienpunkt
//             memcpy(_segmentStartSteps, _trajectory[_currentSegmentIdx].steps, sizeof(_segmentStartSteps));

//             if (_currentSegmentIdx < _trajectory.size() - 1)
//             {
//                 // Ende = nächster Trajektorienpunkt
//                 _segmentEndTime = _trajectory[_currentSegmentIdx + 1].timeMs;
//                 memcpy(_segmentEndSteps, _trajectory[_currentSegmentIdx + 1].steps, sizeof(_segmentEndSteps));
//             }
//             else
//             {
//                 // Letztes Segment
//                 _segmentEndTime = _totalTimeMs;
//                 memcpy(_segmentEndSteps, _targetSteps, sizeof(_segmentEndSteps));
//             }

//             // Debug: Segment-Wechsel (alle 10 Segmente)
//             if (_currentSegmentIdx % 10 == 0)
//             {
//                 Serial.printf("Segment %d: t=%dms to t=%dms\n",
//                               _currentSegmentIdx, _segmentStartTime, _segmentEndTime);
//             }
//         }
//     }

//     // 3. Interpoliertes Ziel für aktuelle Zeit berechnen
//     _updateInterpolatedTarget();

//     // 4. Wenn sich interpoliertes Ziel geändert hat, Bresenham aktualisieren
//     static int32_t lastInterpolated[6] = {0};
//     bool           targetChanged       = false;

//     for (int i = 0; i < 6; i++)
//     {
//         if (_interpolatedTarget[i] != lastInterpolated[i])
//         {
//             targetChanged       = true;
//             lastInterpolated[i] = _interpolatedTarget[i];
//         }
//     }

//     if (targetChanged)
//     {
//         _updateBresenhamToInterpolated();
//     }

//     // 5. Bresenham-Schritt ausführen
//     _executeBresenhamStep();

//     // 6. Debug-Logging (alle 500ms)
//     static uint32_t lastLogTime = 0;
//     if ((micros() / 1000) - lastLogTime > 500)
//     {
//         lastLogTime = micros() / 1000;
//         Serial.printf("t=%dms: Pos=[%d,%d], Target=[%d,%d], Interp=[%d,%d], Seg=%d\n",
//                       elapsedTime,
//                       _currentSteps[0], _currentSteps[1],
//                       _targetSteps[0], _targetSteps[1],
//                       _interpolatedTarget[0], _interpolatedTarget[1],
//                       _currentSegmentIdx);
//     }

//     // 7. Ende-Check
//     if (elapsedTime >= _totalTimeMs)
//     {
//         // Prüfen ob alle Achsen am finalen Ziel sind
//         bool    allAtTarget = true;
//         int32_t maxError    = 0;

//         for (int i = 0; i < 6; i++)
//         {
//             int32_t error = abs(_currentSteps[i] - _targetSteps[i]);
//             if (error > maxError)
//                 maxError = error;
//             if (error > 5) // Tolerance: 5 steps
//             {
//                 allAtTarget = false;
//             }
//         }

//         if (allAtTarget || maxError < 5)
//         {
//             _stepTimer.end();
//             _isRunning = false;

//             // Finale Position erzwingen (für perfekte Genauigkeit)
//             memcpy(_currentSteps, _targetSteps, sizeof(_currentSteps));

//             // AccelStepper Positionen synchronisieren
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             Serial.println("\n=== TRAJECTORY COMPLETE ===");
//             Serial.printf("Final positions: [%d,%d,%d,%d,%d,%d]\n",
//                           _currentSteps[0], _currentSteps[1], _currentSteps[2],
//                           _currentSteps[3], _currentSteps[4], _currentSteps[5]);
//             Serial.printf("Max error: %d steps\n", maxError);
//             Serial.printf("Actual duration: %d ms\n\n", elapsedTime);

//             // INFO_REACHED senden
//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//         else if (elapsedTime > _totalTimeMs + 500) // 500ms Timeout
//         {
//             // Notfall: Zeit überschritten aber nicht am Ziel
//             Serial.printf("WARNING: Timeout! Still %d steps from target\n", maxError);

//             _stepTimer.end();
//             _isRunning = false;

//             // AccelStepper mit aktueller Position synchronisieren
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//     }
// }

// void TrajectoryExecutor::_updateInterpolatedTarget()
// {
//     // Aktuelle Zeit im Segment - micros() für Präzision
//     uint32_t elapsedTime = (micros() / 1000) - _trajectoryStartTime;

//     // Begrenze auf Segment-Zeitbereich
//     uint32_t timeInSegment = elapsedTime;
//     if (timeInSegment < _segmentStartTime)
//         timeInSegment = _segmentStartTime;
//     if (timeInSegment > _segmentEndTime)
//         timeInSegment = _segmentEndTime;

//     uint32_t segmentDuration = _segmentEndTime - _segmentStartTime;

//     if (segmentDuration == 0)
//     {
//         // Kein Zeitunterschied - direkt ans Ziel
//         memcpy(_interpolatedTarget, _segmentEndSteps, sizeof(_interpolatedTarget));
//         return;
//     }

//     // Lineare Interpolation: t = 0.0 (Start) bis 1.0 (Ende)
//     float t = (float)(timeInSegment - _segmentStartTime) / (float)segmentDuration;
//     t       = constrain(t, 0.0f, 1.0f);

//     // Für jede Achse interpolieren
//     for (int i = 0; i < 6; i++)
//     {
//         int32_t startPos       = _segmentStartSteps[i];
//         int32_t endPos         = _segmentEndSteps[i];
//         _interpolatedTarget[i] = startPos + (int32_t)((endPos - startPos) * t);
//     }
// }

// void TrajectoryExecutor::_updateBresenhamToInterpolated()
// {
//     // Delta berechnen (von aktueller Position zum interpolierten Ziel)
//     _bresenhamMaxDelta = 0;

//     for (int i = 0; i < 6; i++)
//     {
//         _bresenhamDelta[i] = abs(_interpolatedTarget[i] - _currentSteps[i]);

//         if (_bresenhamDelta[i] > _bresenhamMaxDelta)
//         {
//             _bresenhamMaxDelta = _bresenhamDelta[i];
//         }
//     }

//     if (_bresenhamMaxDelta == 0)
//         _bresenhamMaxDelta = 1;

//     // Fehlerterme neu initialisieren
//     for (int i = 0; i < 6; i++)
//     {
//         _bresenhamError[i] = _bresenhamMaxDelta / 2;
//     }

//     // Richtungen setzen
//     for (int i = 0; i < 6; i++)
//     {
//         bool dir = (_interpolatedTarget[i] > _currentSteps[i]);
//         _setDirection(i, dir);
//     }
// }

// void TrajectoryExecutor::_executeBresenhamStep()
// {
//     for (int i = 0; i < 6; i++)
//     {
//         // Wenn bereits am interpolierten Ziel, nichts tun
//         if (_currentSteps[i] == _interpolatedTarget[i])
//             continue;

//         _bresenhamError[i] += _bresenhamDelta[i];

//         if (_bresenhamError[i] >= _bresenhamMaxDelta)
//         {
//             _bresenhamError[i] -= _bresenhamMaxDelta;

//             // Richtung prüfen und Step machen
//             if (_interpolatedTarget[i] > _currentSteps[i])
//             {
//                 _pulseStep(i);
//                 _currentSteps[i]++;
//             }
//             else if (_interpolatedTarget[i] < _currentSteps[i])
//             {
//                 _pulseStep(i);
//                 _currentSteps[i]--;
//             }
//         }
//     }
// }

// void TrajectoryExecutor::_pulseStep(uint8_t motorIdx)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->stepPin;
//     digitalWriteFast(pin, HIGH);
//     delayMicroseconds(2);
//     digitalWriteFast(pin, LOW);
// }

// void TrajectoryExecutor::_setDirection(uint8_t motorIdx, bool direction)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->dirPin;
//     digitalWriteFast(pin, direction ? HIGH : LOW);
// }

// void TrajectoryExecutor::_staticStepTimerISR()
// {
//     if (_instance)
//     {
//         _instance->_stepTimerISR();
//     }
// }

// void TrajectoryExecutor::cancel()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
//     _isRunning = false;
//     Serial.println("Trajectory cancelled");
// }

// void TrajectoryExecutor::emergencyStop()
// {
//     cancel();
//     for (auto& config : _motorConfigs)
//     {
//         if (config)
//         {
//             digitalWrite(config->stepPin, LOW);
//         }
//     }
// }

// void TrajectoryExecutor::update()
// {
//     // Optional: Position-Updates außerhalb ISR senden
// }

// float TrajectoryExecutor::getProgress() const
// {
//     if (!_isRunning || _totalTimeMs == 0)
//         return 0.0f;

//     uint32_t elapsed = (micros() / 1000) - _trajectoryStartTime;
//     return min(100.0f, (elapsed * 100.0f) / _totalTimeMs);
// }
//!!!!!!!!!!!
// #include "TrajectoryExecutor.h"
// #include "ComProtocol.h"
// #include "Utils.h"
// #include <cstring>

// using namespace CommunicationProtocoll;

// TrajectoryExecutor* TrajectoryExecutor::_instance = nullptr;

// TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
//     : _motorConfigs(motorConfigs)
// {
//     _instance = this;
//     memset(_currentSteps, 0, sizeof(_currentSteps));
//     memset(_targetSteps, 0, sizeof(_targetSteps));
//     memset(_interpolatedTarget, 0, sizeof(_interpolatedTarget));
//     memset(_bresenhamError, 0, sizeof(_bresenhamError));
//     memset(_bresenhamDelta, 0, sizeof(_bresenhamDelta));
// }

// TrajectoryExecutor::~TrajectoryExecutor()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
// }

// void TrajectoryExecutor::begin()
// {
//     return;
//     // Serial.println("TrajectoryExecutor initialized");
//     // Serial.printf("Timer frequency: %d Hz (%d us period)\n",
//     //               1000000 / STEP_TIMER_PERIOD_US, STEP_TIMER_PERIOD_US);
// }

// void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
// {
//     _trajectory.clear();
//     _trajectory.reserve(numPoints);
//     _isUploading = true;
//     _totalTimeMs = totalTimeMs;
// }

// void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
// {
//     if (!_isUploading)
//         return;

//     TrajectoryPoint point;
//     point.timeMs = timeMs;
//     memcpy(point.steps, steps, 6 * sizeof(int32_t));

//     _trajectory.push_back(point);
// }

// void TrajectoryExecutor::endUpload()
// {
//     _isUploading      = false;
//     _trajectoryLoaded = true;
// }

// void TrajectoryExecutor::execute()
// {
//     if (!_trajectoryLoaded || _trajectory.empty())
//     {
//         return;
//     }

//     // Timer stoppen falls läuft
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }

//     // WICHTIG: Aktuelle physische Position aus AccelStepper holen
//     for (size_t i = 0; i < 6; i++)
//     {
//         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//         {
//             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
//         }
//     }

//     // Finales Ziel (letzter Trajektorienpunkt)
//     memcpy(_targetSteps, _trajectory.back().steps, sizeof(_targetSteps));

//     // Erstes Segment initialisieren: Von aktueller Position zum ersten Punkt
//     _currentSegmentIdx = 0;
//     _segmentStartTime  = 0;
//     _segmentEndTime    = _trajectory[0].timeMs;

//     // Segment-Start = AKTUELLE Position (wo Robot jetzt ist)
//     memcpy(_segmentStartSteps, _currentSteps, sizeof(_segmentStartSteps));

//     // Segment-Ende = erster Trajektorienpunkt
//     memcpy(_segmentEndSteps, _trajectory[0].steps, sizeof(_segmentEndSteps));

//     // Interpoliertes Ziel initial = aktuelle Position
//     memcpy(_interpolatedTarget, _currentSteps, sizeof(_interpolatedTarget));

//     // Bresenham initialisieren
//     _updateBresenhamToInterpolated();

//     // Ausführung starten
//     _trajectoryStartTime = millis();
//     _isRunning           = true;

//     // Timer starten
//     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);
// }

// void TrajectoryExecutor::_stepTimerISR()
// {
//     if (!_isRunning || _trajectory.empty())
//         return;

//     // 1. Aktuelle Zeit seit Start
//     uint32_t elapsedTime = millis() - _trajectoryStartTime;

//     // 2. Prüfen ob wir zum nächsten Segment wechseln müssen
//     if (_currentSegmentIdx < _trajectory.size() - 1)
//     {
//         uint32_t nextSegmentTime = _trajectory[_currentSegmentIdx + 1].timeMs;

//         if (elapsedTime >= nextSegmentTime)
//         {
//             // Nächstes Segment
//             _currentSegmentIdx++;

//             // Segment-Grenzen aktualisieren
//             _segmentStartTime = _trajectory[_currentSegmentIdx].timeMs;

//             // Start = aktueller Trajektorienpunkt
//             memcpy(_segmentStartSteps, _trajectory[_currentSegmentIdx].steps, sizeof(_segmentStartSteps));

//             if (_currentSegmentIdx < _trajectory.size() - 1)
//             {
//                 // Ende = nächster Trajektorienpunkt
//                 _segmentEndTime = _trajectory[_currentSegmentIdx + 1].timeMs;
//                 memcpy(_segmentEndSteps, _trajectory[_currentSegmentIdx + 1].steps, sizeof(_segmentEndSteps));
//             }
//             else
//             {
//                 // Letztes Segment
//                 _segmentEndTime = _totalTimeMs;
//                 memcpy(_segmentEndSteps, _targetSteps, sizeof(_segmentEndSteps));
//             }
//         }
//     }

//     // 3. Interpoliertes Ziel für aktuelle Zeit berechnen
//     _updateInterpolatedTarget();

//     // 4. Wenn sich interpoliertes Ziel geändert hat, Bresenham aktualisieren
//     static int32_t lastInterpolated[6] = {0};
//     bool           targetChanged       = false;

//     for (int i = 0; i < 6; i++)
//     {
//         if (_interpolatedTarget[i] != lastInterpolated[i])
//         {
//             targetChanged       = true;
//             lastInterpolated[i] = _interpolatedTarget[i];
//         }
//     }

//     if (targetChanged)
//     {
//         _updateBresenhamToInterpolated();
//     }

//     // 5. Bresenham-Schritt ausführen
//     _executeBresenhamStep();

//     // 7. Ende-Check
//     if (elapsedTime >= _totalTimeMs)
//     {
//         // Prüfen ob alle Achsen am finalen Ziel sind
//         bool    allAtTarget = true;
//         int32_t maxError    = 0;

//         for (int i = 0; i < 6; i++)
//         {
//             int32_t error = abs(_currentSteps[i] - _targetSteps[i]);
//             if (error > maxError)
//                 maxError = error;
//             if (error > 5) // Tolerance: 5 steps
//             {
//                 allAtTarget = false;
//             }
//         }

//         if (allAtTarget || maxError < 5)
//         {
//             _stepTimer.end();
//             _isRunning = false;

//             // Finale Position erzwingen (für perfekte Genauigkeit)
//             memcpy(_currentSteps, _targetSteps, sizeof(_currentSteps));

//             // AccelStepper Positionen synchronisieren
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             // INFO_REACHED senden
//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//         else if (elapsedTime > _totalTimeMs + 500) // 500ms Timeout
//         {
//             // Notfall: Zeit überschritten aber nicht am Ziel

//             _stepTimer.end();
//             _isRunning = false;

//             // AccelStepper mit aktueller Position synchronisieren
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//     }
// }

// void TrajectoryExecutor::_updateInterpolatedTarget()
// {
//     // Aktuelle Zeit im Segment
//     uint32_t elapsedTime = millis() - _trajectoryStartTime;

//     // Begrenze auf Segment-Zeitbereich
//     uint32_t timeInSegment = elapsedTime;
//     if (timeInSegment < _segmentStartTime)
//         timeInSegment = _segmentStartTime;
//     if (timeInSegment > _segmentEndTime)
//         timeInSegment = _segmentEndTime;

//     uint32_t segmentDuration = _segmentEndTime - _segmentStartTime;

//     if (segmentDuration == 0)
//     {
//         // Kein Zeitunterschied - direkt ans Ziel
//         memcpy(_interpolatedTarget, _segmentEndSteps, sizeof(_interpolatedTarget));
//         return;
//     }

//     // Lineare Interpolation: t = 0.0 (Start) bis 1.0 (Ende)
//     float t = (float)(timeInSegment - _segmentStartTime) / (float)segmentDuration;
//     t       = constrain(t, 0.0f, 1.0f);

//     // Für jede Achse interpolieren
//     for (int i = 0; i < 6; i++)
//     {
//         int32_t startPos       = _segmentStartSteps[i];
//         int32_t endPos         = _segmentEndSteps[i];
//         _interpolatedTarget[i] = startPos + (int32_t)((endPos - startPos) * t);
//     }
// }

// void TrajectoryExecutor::_updateBresenhamToInterpolated()
// {
//     // Delta berechnen (von aktueller Position zum interpolierten Ziel)
//     _bresenhamMaxDelta = 0;

//     for (int i = 0; i < 6; i++)
//     {
//         _bresenhamDelta[i] = abs(_interpolatedTarget[i] - _currentSteps[i]);

//         if (_bresenhamDelta[i] > _bresenhamMaxDelta)
//         {
//             _bresenhamMaxDelta = _bresenhamDelta[i];
//         }
//     }

//     if (_bresenhamMaxDelta == 0)
//         _bresenhamMaxDelta = 1;

//     // Fehlerterme neu initialisieren
//     for (int i = 0; i < 6; i++)
//     {
//         _bresenhamError[i] = _bresenhamMaxDelta / 2;
//     }

//     // Richtungen setzen
//     for (int i = 0; i < 6; i++)
//     {
//         bool dir = (_interpolatedTarget[i] > _currentSteps[i]);
//         _setDirection(i, dir);
//     }
// }

// void TrajectoryExecutor::_executeBresenhamStep()
// {
//     for (int i = 0; i < 6; i++)
//     {
//         // Wenn bereits am interpolierten Ziel, nichts tun
//         if (_currentSteps[i] == _interpolatedTarget[i])
//             continue;

//         _bresenhamError[i] += _bresenhamDelta[i];

//         if (_bresenhamError[i] >= _bresenhamMaxDelta)
//         {
//             _bresenhamError[i] -= _bresenhamMaxDelta;

//             // Richtung prüfen und Step machen
//             if (_interpolatedTarget[i] > _currentSteps[i])
//             {
//                 _pulseStep(i);
//                 _currentSteps[i]++;
//             }
//             else if (_interpolatedTarget[i] < _currentSteps[i])
//             {
//                 _pulseStep(i);
//                 _currentSteps[i]--;
//             }
//         }
//     }
// }

// void TrajectoryExecutor::_pulseStep(uint8_t motorIdx)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->stepPin;
//     digitalWriteFast(pin, HIGH);
//     delayMicroseconds(2);
//     digitalWriteFast(pin, LOW);
// }

// void TrajectoryExecutor::_setDirection(uint8_t motorIdx, bool direction)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->dirPin;
//     digitalWriteFast(pin, direction ? HIGH : LOW);
// }

// void TrajectoryExecutor::_staticStepTimerISR()
// {
//     if (_instance)
//     {
//         _instance->_stepTimerISR();
//     }
// }

// void TrajectoryExecutor::cancel()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
//     _isRunning = false;
// }

// void TrajectoryExecutor::emergencyStop()
// {
//     cancel();
//     for (auto& config : _motorConfigs)
//     {
//         if (config)
//         {
//             digitalWrite(config->stepPin, LOW);
//         }
//     }
// }

// void TrajectoryExecutor::update()
// {
//     // Optional: Position-Updates außerhalb ISR senden
// }

// float TrajectoryExecutor::getProgress() const
// {
//     if (!_isRunning || _totalTimeMs == 0)
//         return 0.0f;

//     uint32_t elapsed = millis() - _trajectoryStartTime;
//     return min(100.0f, (elapsed * 100.0f) / _totalTimeMs);
// }

// //!!!!
// #include "TrajectoryExecutor.h"
// #include "ComProtocol.h"
// #include "Utils.h"
// #include <cstring>

// using namespace CommunicationProtocoll;

// TrajectoryExecutor* TrajectoryExecutor::_instance = nullptr;

// TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
//     : _motorConfigs(motorConfigs)
// {
//     _instance = this;
//     memset(_currentSteps, 0, sizeof(_currentSteps));
//     memset(_targetSteps, 0, sizeof(_targetSteps));
//     memset(_interpolatedTarget, 0, sizeof(_interpolatedTarget));
//     memset(_bresenhamError, 0, sizeof(_bresenhamError));
//     memset(_bresenhamDelta, 0, sizeof(_bresenhamDelta));
// }

// TrajectoryExecutor::~TrajectoryExecutor()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
// }

// void TrajectoryExecutor::begin()
// {
//     Serial.println("TrajectoryExecutor initialized");
// }

// void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
// {
//     _trajectory.clear();
//     _trajectory.reserve(numPoints);
//     _isUploading = true;
//     _totalTimeMs = totalTimeMs;
// }

// void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
// {
//     if (!_isUploading)
//         return;

//     TrajectoryPoint point;
//     point.timeMs = timeMs;
//     memcpy(point.steps, steps, 6 * sizeof(int32_t));

//     _trajectory.push_back(point);
// }

// void TrajectoryExecutor::endUpload()
// {
//     _isUploading      = false;
//     _trajectoryLoaded = true;
// }

// void TrajectoryExecutor::execute()
// {
//     if (!_trajectoryLoaded || _trajectory.empty())
//     {
//         return;
//     }

//     // Timer stoppen falls läuft
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }

//     // WICHTIG: Aktuelle physische Position aus AccelStepper holen
//     for (size_t i = 0; i < 6; i++)
//     {
//         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//         {
//             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
//         }
//     }

//     // Finales Ziel setzen (letzter Punkt)
//     memcpy(_targetSteps, _trajectory.back().steps, sizeof(_targetSteps));

//     // Erstes Segment initialisieren
//     _currentSegmentIdx = 0;

//     // Segment-Start = aktuelle physische Position (NICHT erster Trajektorienpunkt!)
//     memcpy(_segmentStartSteps, _currentSteps, sizeof(_segmentStartSteps));

//     // Segment-Ende = erster Trajektorienpunkt
//     if (_trajectory.size() > 0)
//     {
//         memcpy(_segmentEndSteps, _trajectory[0].steps, sizeof(_segmentEndSteps));
//         _segmentStartTime = 0;
//         _segmentEndTime   = _trajectory[0].timeMs;
//     }

//     // Interpoliertes Ziel initial = aktuelle Position
//     memcpy(_interpolatedTarget, _currentSteps, sizeof(_interpolatedTarget));

//     // Bresenham initial für ersten Schritt
//     _updateBresenhamToInterpolated();

//     // Ausführung starten
//     _trajectoryStartTime = millis();
//     _isRunning           = true;

//     // Timer starten
//     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);
// }

// void TrajectoryExecutor::cancel()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
//     _isRunning = false;
// }

// void TrajectoryExecutor::emergencyStop()
// {
//     cancel();
//     for (auto& config : _motorConfigs)
//     {
//         if (config)
//         {
//             digitalWrite(config->stepPin, LOW);
//         }
//     }
// }

// void TrajectoryExecutor::_stepTimerISR()
// {
//     if (!_isRunning || _trajectory.empty())
//         return;

//     // 1. Aktuelle Zeit seit Start
//     uint32_t elapsedTime = millis() - _trajectoryStartTime;

//     // 2. Prüfen ob wir zum nächsten Segment wechseln müssen
//     if (_currentSegmentIdx < _trajectory.size() - 1)
//     {
//         uint32_t nextSegmentTime = _trajectory[_currentSegmentIdx + 1].timeMs;

//         if (elapsedTime >= nextSegmentTime)
//         {
//             // Nächstes Segment
//             _currentSegmentIdx++;

//             // Segment-Grenzen aktualisieren
//             memcpy(_segmentStartSteps, _trajectory[_currentSegmentIdx].steps, sizeof(_segmentStartSteps));

//             if (_currentSegmentIdx < _trajectory.size() - 1)
//             {
//                 memcpy(_segmentEndSteps, _trajectory[_currentSegmentIdx + 1].steps, sizeof(_segmentEndSteps));
//                 _segmentStartTime = _trajectory[_currentSegmentIdx].timeMs;
//                 _segmentEndTime   = _trajectory[_currentSegmentIdx + 1].timeMs;
//             }
//             else
//             {
//                 // Letztes Segment - Ziel ist finaler Punkt
//                 memcpy(_segmentEndSteps, _targetSteps, sizeof(_segmentEndSteps));
//                 _segmentStartTime = _trajectory[_currentSegmentIdx].timeMs;
//                 _segmentEndTime   = _totalTimeMs;
//             }
//         }
//     }

//     // 3. Interpoliertes Ziel für aktuelle Zeit berechnen
//     _updateInterpolatedTarget();

//     // 4. Wenn sich interpoliertes Ziel geändert hat, Bresenham neu berechnen
//     static int32_t lastInterpolated[6] = {0};
//     bool           targetChanged       = false;

//     for (int i = 0; i < 6; i++)
//     {
//         if (_interpolatedTarget[i] != lastInterpolated[i])
//         {
//             targetChanged       = true;
//             lastInterpolated[i] = _interpolatedTarget[i];
//         }
//     }

//     if (targetChanged)
//     {
//         _updateBresenhamToInterpolated();
//     }

//     // 5. Bresenham-Schritt ausführen (zum interpolierten Ziel)
//     _executeBresenhamStep();

//     // 7. Ende-Check
//     if (elapsedTime >= _totalTimeMs)
//     {
//         // Prüfen ob alle Achsen am finalen Ziel sind
//         bool allAtTarget = true;
//         for (int i = 0; i < 6; i++)
//         {
//             if (_currentSteps[i] != _targetSteps[i])
//             {
//                 allAtTarget = false;
//                 break;
//             }
//         }

//         if (allAtTarget)
//         {
//             _stepTimer.end();
//             _isRunning = false;

//             // AccelStepper Positionen synchronisieren
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             // INFO_REACHED senden
//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//     }
// }

// void TrajectoryExecutor::_updateInterpolatedTarget()
// {
//     // Aktuelle Zeit im Segment
//     uint32_t elapsedTime = millis() - _trajectoryStartTime;

//     // Begrenze auf Segment-Zeitbereich
//     if (elapsedTime < _segmentStartTime)
//         elapsedTime = _segmentStartTime;
//     if (elapsedTime > _segmentEndTime)
//         elapsedTime = _segmentEndTime;

//     uint32_t segmentDuration = _segmentEndTime - _segmentStartTime;

//     if (segmentDuration == 0)
//     {
//         // Kein Zeitunterschied - direkt ans Ziel
//         memcpy(_interpolatedTarget, _segmentEndSteps, sizeof(_interpolatedTarget));
//         return;
//     }

//     // Lineare Interpolation: t = 0.0 (Start) bis 1.0 (Ende)
//     float t = (float)(elapsedTime - _segmentStartTime) / (float)segmentDuration;
//     t       = constrain(t, 0.0f, 1.0f);

//     // Für jede Achse interpolieren
//     for (int i = 0; i < 6; i++)
//     {
//         int32_t startPos       = _segmentStartSteps[i];
//         int32_t endPos         = _segmentEndSteps[i];
//         _interpolatedTarget[i] = startPos + (int32_t)((endPos - startPos) * t);
//     }
// }

// void TrajectoryExecutor::_updateBresenhamToInterpolated()
// {
//     // Delta berechnen (von aktueller Position zum interpolierten Ziel)
//     _bresenhamMaxDelta = 0;

//     for (int i = 0; i < 6; i++)
//     {
//         _bresenhamDelta[i] = abs(_interpolatedTarget[i] - _currentSteps[i]);

//         if (_bresenhamDelta[i] > _bresenhamMaxDelta)
//         {
//             _bresenhamMaxDelta = _bresenhamDelta[i];
//         }
//     }

//     if (_bresenhamMaxDelta == 0)
//         _bresenhamMaxDelta = 1;

//     // Fehlerterme neu initialisieren
//     for (int i = 0; i < 6; i++)
//     {
//         _bresenhamError[i] = _bresenhamMaxDelta / 2;
//     }

//     // Richtungen setzen
//     for (int i = 0; i < 6; i++)
//     {
//         bool dir = (_interpolatedTarget[i] > _currentSteps[i]);
//         _setDirection(i, dir);
//     }
// }

// void TrajectoryExecutor::_executeBresenhamStep()
// {
//     for (int i = 0; i < 6; i++)
//     {
//         // Wenn bereits am interpolierten Ziel, nichts tun
//         if (_currentSteps[i] == _interpolatedTarget[i])
//             continue;

//         _bresenhamError[i] += _bresenhamDelta[i];

//         if (_bresenhamError[i] >= _bresenhamMaxDelta)
//         {
//             _bresenhamError[i] -= _bresenhamMaxDelta;

//             // Richtung prüfen und Step machen
//             if (_interpolatedTarget[i] > _currentSteps[i])
//             {
//                 _pulseStep(i);
//                 _currentSteps[i]++;
//             }
//             else if (_interpolatedTarget[i] < _currentSteps[i])
//             {
//                 _pulseStep(i);
//                 _currentSteps[i]--;
//             }
//         }
//     }
// }

// void TrajectoryExecutor::_pulseStep(uint8_t motorIdx)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->stepPin;
//     digitalWriteFast(pin, HIGH);
//     delayMicroseconds(2); // Mindestens 1-2µs für Treiber
//     digitalWriteFast(pin, LOW);
// }

// void TrajectoryExecutor::_setDirection(uint8_t motorIdx, bool direction)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->dirPin;
//     digitalWriteFast(pin, direction ? HIGH : LOW);
// }

// void TrajectoryExecutor::_staticStepTimerISR()
// {
//     if (_instance)
//     {
//         _instance->_stepTimerISR();
//     }
// }

// void TrajectoryExecutor::update()
// {
//     // Position-Updates für Frontend (außerhalb ISR)
//     static uint32_t lastSendTime = 0;
//     uint32_t        now          = millis();

//     if (_isRunning && (now - lastSendTime > 50))
//     {
//         // Hier könnte man Positions-Updates senden
//         lastSendTime = now;
//     }
// }

// float TrajectoryExecutor::getProgress() const
// {
//     if (!_isRunning || _totalTimeMs == 0)
//         return 0.0f;

//     uint32_t elapsed = millis() - _trajectoryStartTime;
//     return min(100.0f, (elapsed * 100.0f) / _totalTimeMs);
// }

//!!!
// #include "TrajectoryExecutor.h"
// #include "ComProtocol.h"
// #include "Utils.h"
// #include <cstring>

// using namespace CommunicationProtocoll;

// TrajectoryExecutor* TrajectoryExecutor::_instance = nullptr;

// TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
//     : _motorConfigs(motorConfigs)
// {
//     _instance = this;
//     memset(_currentSteps, 0, sizeof(_currentSteps));
//     memset(_targetSteps, 0, sizeof(_targetSteps));
//     memset(_error, 0, sizeof(_error));
//     memset(_delta, 0, sizeof(_delta));
//     _maxDelta = 1;
// }

// TrajectoryExecutor::~TrajectoryExecutor()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
// }

// void TrajectoryExecutor::begin()
// {
//     Serial.println("TrajectoryExecutor initialized");
// }

// void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
// {
//     _trajectory.clear();
//     _trajectory.reserve(numPoints);
//     _isUploading = true;
//     _totalTimeMs = totalTimeMs;

//     Serial.printf("Trajectory upload start: %d points, %d ms\n", numPoints, totalTimeMs);
// }

// void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
// {
//     if (!_isUploading)
//         return;

//     TrajectoryPoint point;
//     point.timeMs = timeMs;
//     memcpy(point.steps, steps, 6 * sizeof(int32_t));

//     _trajectory.push_back(point);
// }

// void TrajectoryExecutor::endUpload()
// {
//     _isUploading      = false;
//     _trajectoryLoaded = true;
//     // Serial.printf("Trajectory upload complete: %d points loaded\n", _trajectory.size());

//     // // Debug: Ersten und letzten Punkt anzeigen
//     // if (!_trajectory.empty())
//     // {
//     //     const auto& first = _trajectory.front();
//     //     const auto& last  = _trajectory.back();
//     //     Serial.printf("First point: time=%d, steps=[%d,%d,%d,%d,%d,%d]\n",
//     //                   first.timeMs, first.steps[0], first.steps[1], first.steps[2],
//     //                   first.steps[3], first.steps[4], first.steps[5]);
//     //     Serial.printf("Last point: time=%d, steps=[%d,%d,%d,%d,%d,%d]\n",
//     //                   last.timeMs, last.steps[0], last.steps[1], last.steps[2],
//     //                   last.steps[3], last.steps[4], last.steps[5]);
//     // }
// }

// void TrajectoryExecutor::execute()
// {
//     if (!_trajectoryLoaded || _trajectory.empty())
//     {
//         Serial.println("Cannot execute: No trajectory loaded!");
//         return;
//     }

//     // Timer stoppen falls läuft
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }

//     // Startzustand
//     _trajectoryStartTime = millis();
//     _currentPointIdx     = 0;
//     _isRunning           = true;

//     // Aktuelle Position aus AccelStepper holen
//     for (size_t i = 0; i < 6; i++)
//     {
//         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//         {
//             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
//             // Serial.printf("Motor %d start position: %d\n", i, _currentSteps[i]);
//         }
//     }

//     // Erstes Ziel interpolieren (Zeit = 0)
//     _interpolateTrajectory(0);

//     // Bresenham-Parameter initialisieren
//     _updateBresenhamParameters();

//     // Fehler initialisieren
//     for (int i = 0; i < 6; i++)
//     {
//         _error[i] = _maxDelta / 2;
//     }

//     // Timer starten (100Hz = 10ms für Interpolation)
//     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);
//     Serial.println("Trajectory execution started!");
// }

// void TrajectoryExecutor::cancel()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
//     _isRunning = false;
//     Serial.println("Trajectory cancelled");
// }

// void TrajectoryExecutor::emergencyStop()
// {
//     cancel();
//     // Alle STEP-Pins auf LOW
//     for (auto& config : _motorConfigs)
//     {
//         if (config)
//         {
//             digitalWrite(config->stepPin, LOW);
//         }
//     }
// }

// void TrajectoryExecutor::_stepTimerISR()
// {
//     if (!_isRunning || _trajectory.empty())
//         return;

//     // 1. Aktuelle Zeit berechnen
//     uint32_t currentTime = millis() - _trajectoryStartTime;

//     // 2. Zielposition für aktuelle Zeit interpolieren
//     _interpolateTrajectory(currentTime);

//     // 3. Bresenham-Parameter aktualisieren (nur wenn sich das Ziel geändert hat)
//     _updateBresenhamParameters();

//     // 4. Bresenham-Schritt zum Ziel ausführen
//     _doBresenhamStepToTarget();

//     // 5. Prüfen ob Trajektorie beendet
//     if (currentTime >= _totalTimeMs)
//     {
//         // Noch prüfen ob alle Motoren am Ziel sind
//         bool allDone = true;
//         for (int i = 0; i < 6; i++)
//         {
//             if (_currentSteps[i] != _targetSteps[i])
//             {
//                 allDone = false;
//                 break;
//             }
//         }

//         if (allDone)
//         {
//             _stepTimer.end();
//             _isRunning = false;

//             // HIER: Steps an AccelStepper zurückgeben
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             // HIER: Position an Frontend senden
//             // z.B.: RobotDataSender::sendMotorPosInSteps(...) aufrufen

//             // INFO_REACHED senden
//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//     }
// }

// void TrajectoryExecutor::_interpolateTrajectory(uint32_t currentTimeMs)
// {
//     if (_trajectory.empty())
//         return;

//     // Begrenze die Zeit auf die Gesamtdauer
//     currentTimeMs = min(currentTimeMs, _totalTimeMs);

//     if (_trajectory.size() == 1)
//     {
//         // Nur ein Punkt: direktes Ziel
//         const TrajectoryPoint& point = _trajectory[0];
//         memcpy(_targetSteps, point.steps, sizeof(_targetSteps));
//         return;
//     }

//     // Finde den aktuellen Punkt-Index basierend auf der Zeit
//     while (_currentPointIdx < _trajectory.size() - 1 &&
//            currentTimeMs >= _trajectory[_currentPointIdx + 1].timeMs)
//     {
//         _currentPointIdx++;
//     }

//     if (_currentPointIdx >= _trajectory.size() - 1)
//     {
//         // Letzter Punkt
//         const TrajectoryPoint& last = _trajectory.back();
//         memcpy(_targetSteps, last.steps, sizeof(_targetSteps));
//         return;
//     }

//     // Lineare Interpolation zwischen den Punkten
//     const TrajectoryPoint& start = _trajectory[_currentPointIdx];
//     const TrajectoryPoint& end   = _trajectory[_currentPointIdx + 1];

//     uint32_t segmentStartTime = start.timeMs;
//     uint32_t segmentEndTime   = end.timeMs;
//     uint32_t segmentDuration  = segmentEndTime - segmentStartTime;

//     if (segmentDuration == 0)
//     {
//         memcpy(_targetSteps, end.steps, sizeof(_targetSteps));
//         return;
//     }

//     float t = (float)(currentTimeMs - segmentStartTime) / (float)segmentDuration;
//     t       = constrain(t, 0.0f, 1.0f);

//     for (int i = 0; i < 6; i++)
//     {
//         int32_t startPos = start.steps[i];
//         int32_t endPos   = end.steps[i];
//         _targetSteps[i]  = startPos + (int32_t)((endPos - startPos) * t);
//     }
// }

// void TrajectoryExecutor::_updateBresenhamParameters()
// {
//     // Delta und Richtungen berechnen
//     _maxDelta = 0;

//     for (int i = 0; i < 6; i++)
//     {
//         _delta[i] = abs(_targetSteps[i] - _currentSteps[i]);

//         if (_delta[i] > _maxDelta)
//         {
//             _maxDelta = _delta[i];
//         }

//         // Richtung setzen
//         bool dir = (_targetSteps[i] > _currentSteps[i]);
//         _setDirection(i, dir);
//     }

//     if (_maxDelta == 0)
//         _maxDelta = 1;
// }

// void TrajectoryExecutor::_doBresenhamStepToTarget()
// {
//     for (int i = 0; i < 6; i++)
//     {
//         if (_currentSteps[i] == _targetSteps[i])
//             continue;

//         _error[i] += _delta[i];

//         if (_error[i] >= _maxDelta)
//         {
//             _error[i] -= _maxDelta;
//             _pulseStep(i);

//             // Position aktualisieren
//             if (_targetSteps[i] > _currentSteps[i])
//             {
//                 _currentSteps[i]++;
//             }
//             else
//             {
//                 _currentSteps[i]--;
//             }
//         }
//     }
// }

// void TrajectoryExecutor::_pulseStep(uint8_t motorIdx)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->stepPin;
//     digitalWriteFast(pin, HIGH);
//     delayMicroseconds(1);
//     digitalWriteFast(pin, LOW);
// }

// void TrajectoryExecutor::_setDirection(uint8_t motorIdx, bool direction)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->dirPin;
//     digitalWriteFast(pin, direction ? HIGH : LOW);
// }

// void TrajectoryExecutor::_staticStepTimerISR()
// {
//     if (_instance)
//     {
//         _instance->_stepTimerISR();
//     }
// }

// void TrajectoryExecutor::update()
// {
//     // Wird regelmäßig in loop() aufgerufen

//     // HIER könnten Sie regelmäßig die Position an das Frontend senden
//     // z.B. alle 50ms:
//     static uint32_t lastSendTime = 0;
//     uint32_t        now          = millis();

//     if (_isRunning && (now - lastSendTime > 50))
//     {
//         // RobotDataSender::sendMotorPosInSteps(...) aufrufen
//         lastSendTime = now;
//     }
// }

// float TrajectoryExecutor::getProgress() const
// {
//     if (!_isRunning || _totalTimeMs == 0)
//         return 0.0f;

//     uint32_t elapsed = millis() - _trajectoryStartTime;
//     return min(100.0f, (elapsed * 100.0f) / _totalTimeMs);
// }

//!!!!!
// // TrajectoryExecutor.cpp - Korrigierte Version
// #include "TrajectoryExecutor.h"
// #include "ComProtocol.h"
// #include "Utils.h"
// #include <cstring>

// using namespace CommunicationProtocoll;

// TrajectoryExecutor* TrajectoryExecutor::_instance = nullptr;

// TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
//     : _motorConfigs(motorConfigs)
// {
//     _instance = this;
//     memset(_currentSteps, 0, sizeof(_currentSteps));
//     memset(_error, 0, sizeof(_error));
// }

// TrajectoryExecutor::~TrajectoryExecutor()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
// }

// void TrajectoryExecutor::begin()
// {
//     Serial.println("TrajectoryExecutor initialized");
// }

// void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
// {
//     _trajectory.clear();
//     _trajectory.reserve(numPoints);
//     _isUploading = true;
//     _totalTimeMs = totalTimeMs;

//     Serial.printf("Trajectory upload start: %d points, %d ms\n", numPoints, totalTimeMs);
// }

// void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
// {
//     if (!_isUploading)
//         return;

//     TrajectoryPoint point;
//     point.timeMs = timeMs;
//     memcpy(point.steps, steps, 6 * sizeof(int32_t));

//     _trajectory.push_back(point);
// }

// void TrajectoryExecutor::endUpload()
// {
//     _isUploading      = false;
//     _trajectoryLoaded = true;
//     Serial.printf("Trajectory upload complete: %d points loaded\n", _trajectory.size());
// }

// void TrajectoryExecutor::execute()
// {
//     if (!_trajectoryLoaded || _trajectory.empty())
//     {
//         Serial.println("Cannot execute: No trajectory loaded!");
//         return;
//     }

//     // Timer stoppen falls läuft
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }

//     // Startzustand
//     _trajectoryStartTime = millis();
//     _currentPointIdx     = 0;
//     _isRunning           = true;

//     // Aktuelle Position aus AccelStepper holen
//     for (size_t i = 0; i < 6; i++)
//     {
//         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//         {
//             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
//             Serial.printf("Motor %d start position: %d\n", i, _currentSteps[i]);
//         }
//     }

//     // Wenn nur ein Punkt in Trajektorie, direkt zum Ziel bewegen
//     if (_trajectory.size() == 1)
//     {
//         const TrajectoryPoint& target = _trajectory[0];
//         for (int i = 0; i < 6; i++)
//         {
//             _segmentTargetSteps[i] = target.steps[i];
//             _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _currentSteps[i]);
//         }

//         // Max-Delta finden
//         _maxDelta = 0;
//         for (int i = 0; i < 6; i++)
//         {
//             if (_segmentDelta[i] > _maxDelta)
//             {
//                 _maxDelta = _segmentDelta[i];
//             }
//         }

//         if (_maxDelta == 0)
//             _maxDelta = 1;

//         // Bresenham-Fehler initialisieren
//         for (int i = 0; i < 6; i++)
//         {
//             _error[i] = _maxDelta / 2;
//         }

//         // Richtungen setzen
//         for (int i = 0; i < 6; i++)
//         {
//             bool dir = (_segmentTargetSteps[i] > _currentSteps[i]);
//             _setDirection(i, dir);
//         }
//     }
//     else
//     {
//         // Segment initialisieren (von aktueller Position zum ersten Punkt)
//         _updateSegment();
//     }

//     // Timer starten (1000µs = 1kHz)
//     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);
//     Serial.println("Trajectory execution started!");
// }

// void TrajectoryExecutor::cancel()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
//     _isRunning = false;
//     Serial.println("Trajectory cancelled");
// }

// void TrajectoryExecutor::emergencyStop()
// {
//     cancel();
//     // Alle STEP-Pins auf LOW
//     for (auto& config : _motorConfigs)
//     {
//         if (config)
//         {
//             digitalWrite(config->stepPin, LOW);
//         }
//     }
// }

// void TrajectoryExecutor::_stepTimerISR()
// {
//     if (!_isRunning || _trajectory.empty())
//         return;

//     // 1. Prüfen ob nächstes Segment erreicht
//     uint32_t currentTime = millis() - _trajectoryStartTime;

//     if (_trajectory.size() > 1 && _currentPointIdx < _trajectory.size() - 1)
//     {
//         const TrajectoryPoint& nextPoint = _trajectory[_currentPointIdx + 1];

//         if (currentTime >= nextPoint.timeMs)
//         {
//             _currentPointIdx++;

//             if (_currentPointIdx < _trajectory.size() - 1)
//             {
//                 _updateSegment();
//             }
//             else
//             {
//                 // Letztes Segment: von aktuellem Punkt zum letzten Ziel
//                 const TrajectoryPoint& lastPoint = _trajectory[_trajectory.size() - 1];
//                 for (int i = 0; i < 6; i++)
//                 {
//                     _segmentStartSteps[i]  = _currentSteps[i];
//                     _segmentTargetSteps[i] = lastPoint.steps[i];
//                     _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _currentSteps[i]);
//                 }

//                 // Max-Delta finden
//                 _maxDelta = 0;
//                 for (int i = 0; i < 6; i++)
//                 {
//                     if (_segmentDelta[i] > _maxDelta)
//                     {
//                         _maxDelta = _segmentDelta[i];
//                     }
//                 }

//                 if (_maxDelta == 0)
//                     _maxDelta = 1;

//                 // Bresenham-Fehler neu initialisieren
//                 for (int i = 0; i < 6; i++)
//                 {
//                     _error[i] = _maxDelta / 2;
//                 }

//                 // Richtungen setzen
//                 for (int i = 0; i < 6; i++)
//                 {
//                     bool dir = (_segmentTargetSteps[i] > _currentSteps[i]);
//                     _setDirection(i, dir);
//                 }
//             }
//         }
//     }

//     // 2. Bresenham-Schritt ausführen
//     _doBresenhamStep();

//     // 3. Prüfen ob Ende erreicht
//     bool allDone = true;
//     for (int i = 0; i < 6; i++)
//     {
//         if (_currentSteps[i] != _segmentTargetSteps[i])
//         {
//             allDone = false;
//             break;
//         }
//     }

//     if (allDone)
//     {
//         _stepTimer.end();
//         _isRunning = false;

//         // AccelStepper-Positionen aktualisieren
//         for (size_t i = 0; i < 6; i++)
//         {
//             if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//             {
//                 _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//             }
//         }

//         // INFO_REACHED senden
//         Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//     }
// }

// void TrajectoryExecutor::_updateSegment()
// {
//     if (_currentPointIdx >= _trajectory.size() - 1)
//         return;

//     // Nächster Punkt = Ziel
//     const TrajectoryPoint& next = _trajectory[_currentPointIdx + 1];

//     // IMMER von der aktuellen Motorposition starten!
//     for (int i = 0; i < 6; i++)
//     {
//         _segmentStartSteps[i]  = _currentSteps[i]; // Das ist korrekt!
//         _segmentTargetSteps[i] = next.steps[i];
//         _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _segmentStartSteps[i]);
//     }

//     // Max-Delta finden
//     _maxDelta = 0;
//     for (int i = 0; i < 6; i++)
//     {
//         if (_segmentDelta[i] > _maxDelta)
//         {
//             _maxDelta = _segmentDelta[i];
//         }
//     }

//     if (_maxDelta == 0)
//         _maxDelta = 1;

//     // Bresenham-Fehler initialisieren
//     for (int i = 0; i < 6; i++)
//     {
//         _error[i] = _maxDelta / 2;
//     }

//     // Richtungen setzen
//     for (int i = 0; i < 6; i++)
//     {
//         bool dir = (_segmentTargetSteps[i] > _currentSteps[i]);
//         _setDirection(i, dir);
//     }
// }

// // void TrajectoryExecutor::_updateSegment()
// // {
// //     if (_currentPointIdx >= _trajectory.size() - 1)
// //         return;

// //     for (int i = 0; i < 6; i++)
// //     {
// //         _segmentStartSteps[i]  = _currentSteps[i]; // WICHTIG: aktuelle Position!
// //         _segmentTargetSteps[i] = _trajectory[_currentPointIdx + 1].steps[i];
// //         _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _segmentStartSteps[i]);
// //     }

// //     // Aktueller Punkt = Start
// //     const TrajectoryPoint& current = _trajectory[_currentPointIdx];
// //     // Nächster Punkt = Ziel
// //     const TrajectoryPoint& next = _trajectory[_currentPointIdx + 1];

// //     // Für das erste Segment: von aktueller Position zum ersten Punkt
// //     // Für folgende Segmente: von aktuellem Punkt zum nächsten
// //     for (int i = 0; i < 6; i++)
// //     {
// //         if (_currentPointIdx == 0)
// //         {
// //             // Erstes Segment: von aktueller Position zum ersten Punkt
// //             _segmentStartSteps[i] = _currentSteps[i];
// //         }
// //         else
// //         {
// //             _segmentStartSteps[i] = current.steps[i];
// //         }

// //         _segmentTargetSteps[i] = next.steps[i];
// //         _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _segmentStartSteps[i]);
// //     }

// //     // Max-Delta finden
// //     _maxDelta = 0;
// //     for (int i = 0; i < 6; i++)
// //     {
// //         if (_segmentDelta[i] > _maxDelta)
// //         {
// //             _maxDelta = _segmentDelta[i];
// //         }
// //     }

// //     if (_maxDelta == 0)
// //         _maxDelta = 1;

// //     // Bresenham-Fehler initialisieren
// //     for (int i = 0; i < 6; i++)
// //     {
// //         _error[i] = _maxDelta / 2;
// //     }

// //     // Richtungen setzen
// //     for (int i = 0; i < 6; i++)
// //     {
// //         bool dir = (_segmentTargetSteps[i] > _segmentStartSteps[i]);
// //         _setDirection(i, dir);
// //     }
// // }

// void TrajectoryExecutor::_doBresenhamStep()
// {
//     for (int i = 0; i < 6; i++)
//     {
//         if (_currentSteps[i] == _segmentTargetSteps[i])
//             continue;

//         _error[i] += _segmentDelta[i];

//         if (_error[i] >= _maxDelta)
//         {
//             _error[i] -= _maxDelta;
//             _pulseStep(i);

//             // Position aktualisieren
//             if (_segmentTargetSteps[i] > _currentSteps[i])
//             {
//                 _currentSteps[i]++;
//             }
//             else
//             {
//                 _currentSteps[i]--;
//             }
//         }
//     }
// }

// void TrajectoryExecutor::_pulseStep(uint8_t motorIdx)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->stepPin;
//     digitalWriteFast(pin, HIGH);
//     delayMicroseconds(1);
//     digitalWriteFast(pin, LOW);
// }

// void TrajectoryExecutor::_setDirection(uint8_t motorIdx, bool direction)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->dirPin;
//     digitalWriteFast(pin, direction ? HIGH : LOW);
// }

// void TrajectoryExecutor::_staticStepTimerISR()
// {
//     if (_instance)
//     {
//         _instance->_stepTimerISR();
//     }
// }

// void TrajectoryExecutor::update()
// {
//     // Wird regelmäßig in loop() aufgerufen für Status-Updates
// }

// float TrajectoryExecutor::getProgress() const
// {
//     if (!_isRunning || _totalTimeMs == 0)
//         return 0.0f;

//     uint32_t elapsed = millis() - _trajectoryStartTime;
//     return min(100.0f, (elapsed * 100.0f) / _totalTimeMs);
// }
//!!!!!
// // TrajectoryExecutor.cpp - Vollständig camelCase
// #include "TrajectoryExecutor.h"
// #include "ComProtocol.h"
// #include "Utils.h"
// #include <cstring>

// using namespace CommunicationProtocoll;

// TrajectoryExecutor* TrajectoryExecutor::_instance = nullptr;

// TrajectoryExecutor::TrajectoryExecutor(std::vector<MotorConfig*>& motorConfigs)
//     : _motorConfigs(motorConfigs)
// {
//     _instance = this;
//     memset(_currentSteps, 0, sizeof(_currentSteps));
//     memset(_error, 0, sizeof(_error));
// }

// TrajectoryExecutor::~TrajectoryExecutor()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
// }

// void TrajectoryExecutor::begin()
// {
//     // Nichts zu tun, Pins werden in MotorConfigs konfiguriert
//     Serial.println("TrajectoryExecutor initialized");
// }

// void TrajectoryExecutor::startUpload(uint32_t numPoints, uint32_t totalTimeMs)
// {
//     _trajectory.clear();
//     _trajectory.reserve(numPoints);
//     _isUploading = true;
//     _totalTimeMs = totalTimeMs;

//     Serial.printf("Trajectory upload start: %d points, %d ms\n", numPoints, totalTimeMs);
// }

// void TrajectoryExecutor::addPoint(uint32_t timeMs, const int32_t steps[6])
// {
//     if (!_isUploading)
//         return;

//     TrajectoryPoint point;
//     point.timeMs = timeMs;
//     memcpy(point.steps, steps, 6 * sizeof(int32_t));

//     _trajectory.push_back(point);
// }

// void TrajectoryExecutor::endUpload()
// {
//     _isUploading      = false;
//     _trajectoryLoaded = true;
//     Serial.printf("Trajectory upload complete: %d points loaded\n", _trajectory.size());
// }

// void TrajectoryExecutor::execute()
// {
//     if (!_trajectoryLoaded || _trajectory.empty())
//     {
//         Serial.println("Cannot execute: No trajectory loaded!");
//         return;
//     }

//     // Timer stoppen falls läuft
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }

//     // Startzustand
//     _trajectoryStartTime = millis();
//     _currentPointIdx     = 0;
//     _isRunning           = true;

//     // WICHTIG: Aktuelle Position aus AccelStepper holen und BEHALTEN!
//     for (size_t i = 0; i < 6; i++)
//     {
//         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//         {
//             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
//             Serial.printf("Motor %d start position: %d\n", i, _currentSteps[i]);
//         }
//     }

//     // Nur _segmentStartSteps mit erstem Punkt setzen, NICHT _currentSteps!
//     if (!_trajectory.empty())
//     {
//         const TrajectoryPoint& firstPoint = _trajectory[0];
//         for (int i = 0; i < 6; i++)
//         {
//             _segmentStartSteps[i] = firstPoint.steps[i];
//             Serial.printf("Motor %d first trajectory point: %d\n", i, firstPoint.steps[i]);
//         }
//     }

//     // Segment initialisieren
//     if (_trajectory.size() > 1)
//     {
//         _updateSegment();
//     }
//     else
//     {
//         // Für 1-Punkt-Trajektorie
//         const TrajectoryPoint& firstPoint = _trajectory[0];
//         for (int i = 0; i < 6; i++)
//         {
//             _segmentTargetSteps[i] = firstPoint.steps[i];
//             // Delta berechnen von AKTUELLER Position zum Ziel
//             _segmentDelta[i] = abs(_segmentTargetSteps[i] - _currentSteps[i]);
//         }

//         // Max-Delta finden
//         _maxDelta = 0;
//         for (int i = 0; i < 6; i++)
//         {
//             if (_segmentDelta[i] > _maxDelta)
//             {
//                 _maxDelta = _segmentDelta[i];
//             }
//         }

//         // Sicherstellen dass _maxDelta nicht 0 ist
//         if (_maxDelta == 0)
//             _maxDelta = 1;

//         // Bresenham-Fehler initialisieren
//         for (int i = 0; i < 6; i++)
//         {
//             _error[i] = _maxDelta / 2;
//         }

//         // Richtungen setzen
//         for (int i = 0; i < 6; i++)
//         {
//             bool dir = (_segmentTargetSteps[i] > _currentSteps[i]);
//             _setDirection(i, dir);
//             // Serial.printf("Motor %d direction: %s (current: %d, target: %d)\n",
//             //               i, dir ? "HIGH" : "LOW", _currentSteps[i], _segmentTargetSteps[i]);
//         }
//     }

//     // Timer starten (50µs = 20kHz)
//     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);

//     // Serial.println("Trajectory execution started!");
// }

// // void TrajectoryExecutor::execute()
// // {
// //     if (!_trajectoryLoaded || _trajectory.empty())
// //     {
// //         Serial.println("Cannot execute: No trajectory loaded!");
// //         return;
// //     }

// //     // Timer stoppen falls läuft
// //     if (_stepTimer)
// //     {
// //         _stepTimer.end();
// //     }

// //     // Startzustand
// //     _trajectoryStartTime = millis();
// //     _currentPointIdx     = 0;
// //     _isRunning           = true;

// //     // Aktuelle Position aus AccelStepper holen
// //     for (size_t i = 0; i < 6; i++)
// //     {
// //         if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
// //         {
// //             _currentSteps[i] = _motorConfigs[i]->motor->currentPosition();
// //         }
// //     }

// //     // Ersten Punkt als Start setzen
// //     if (!_trajectory.empty())
// //     {
// //         const TrajectoryPoint& firstPoint = _trajectory[0];
// //         for (int i = 0; i < 6; i++)
// //         {
// //             _segmentStartSteps[i] = firstPoint.steps[i];
// //             _currentSteps[i]      = firstPoint.steps[i];
// //         }
// //     }

// //     // Nächstes Segment vorbereiten (falls vorhanden)
// //     if (_trajectory.size() > 1)
// //     {
// //         _updateSegment();
// //     }

// //     // Timer starten (50µs = 20kHz)
// //     _stepTimer.begin(_staticStepTimerISR, STEP_TIMER_PERIOD_US);

// //     Serial.println("Trajectory execution started!");
// // }

// void TrajectoryExecutor::cancel()
// {
//     if (_stepTimer)
//     {
//         _stepTimer.end();
//     }
//     _isRunning = false;
//     Serial.println("Trajectory cancelled");
// }

// void TrajectoryExecutor::emergencyStop()
// {
//     cancel();
//     // Alle STEP-Pins auf LOW
//     for (auto& config : _motorConfigs)
//     {
//         if (config)
//         {
//             digitalWrite(config->stepPin, LOW);
//         }
//     }
// }

// void TrajectoryExecutor::_stepTimerISR()
// {
//     static uint32_t segmentStepCounter = 0;

//     if (!_isRunning || _trajectory.empty())
//         return;

//     // 1. Prüfen ob nächstes Segment erreicht
//     uint32_t currentTime = millis() - _trajectoryStartTime;

//     if (_currentPointIdx < _trajectory.size() - 1)
//     {
//         const TrajectoryPoint& nextPoint = _trajectory[_currentPointIdx + 1];

//         if (currentTime >= nextPoint.timeMs)
//         {
//             _currentPointIdx++;

//             // Neues Segment vorbereiten
//             if (_currentPointIdx < _trajectory.size() - 1)
//             {
//                 _updateSegment();
//                 segmentStepCounter = 0;
//             }
//         }
//     }

//     // 2. Bresenham-Schritt ausführen
//     _doBresenhamStep();

//     segmentStepCounter++;

//     // 3. Prüfen ob Ende erreicht
//     if (_currentPointIdx >= _trajectory.size() - 1)
//     {
//         bool allDone = true;
//         for (int i = 0; i < 6; i++)
//         {
//             if (_currentSteps[i] != _segmentTargetSteps[i])
//             {
//                 allDone = false;
//                 break;
//             }
//         }

//         if (allDone)
//         {
//             _stepTimer.end();
//             _isRunning = false;

//             // WICHTIG: AccelStepper-Positionen aktualisieren!
//             for (size_t i = 0; i < 6; i++)
//             {
//                 if (i < _motorConfigs.size() && _motorConfigs[i]->motor)
//                 {
//                     _motorConfigs[i]->motor->setCurrentPosition(_currentSteps[i]);
//                 }
//             }

//             // INFO_REACHED senden
//             Utils::createAndSendPacket(CMD_TRAJ_EXEC, STATUS_OK, INFO_REACHED);
//         }
//     }
// }

// void TrajectoryExecutor::_updateSegment()
// {
//     if (_currentPointIdx >= _trajectory.size() - 1)
//         return;

//     // Aktueller Punkt = Start
//     const TrajectoryPoint& current = _trajectory[_currentPointIdx];
//     // Nächster Punkt = Ziel
//     const TrajectoryPoint& next = _trajectory[_currentPointIdx + 1];

//     // Zeit für dieses Segment berechnen
//     uint32_t segmentDurationMs = next.timeMs - current.timeMs;
//     if (segmentDurationMs == 0)
//     {
//         segmentDurationMs = 1;
//     }

//     // WICHTIG: Verwende _currentSteps als Start, nicht current.steps!
//     for (int i = 0; i < 6; i++)
//     {
//         _segmentStartSteps[i]  = _currentSteps[i]; // Von AKTUELLER Position
//         _segmentTargetSteps[i] = next.steps[i];
//         _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _currentSteps[i]); // Von AKTUELLER Position
//     }

//     // Max-Delta finden
//     _maxDelta = 0;
//     for (int i = 0; i < 6; i++)
//     {
//         if (_segmentDelta[i] > _maxDelta)
//         {
//             _maxDelta = _segmentDelta[i];
//         }
//     }

//     // WICHTIG: Berechne wie viele ISR-Aufrufe für dieses Segment
//     // Bei 50µs Timer = 20.000 Aufrufe pro Sekunde
//     _segmentTotalSteps = (segmentDurationMs * 20000UL) / 1000UL;
//     if (_segmentTotalSteps == 0)
//         _segmentTotalSteps = 1;

//     // Bresenham-Fehler initialisieren
//     for (int i = 0; i < 6; i++)
//     {
//         _error[i] = _segmentTotalSteps / 2;
//     }

//     // Richtungen setzen
//     for (int i = 0; i < 6; i++)
//     {
//         bool dir = (_segmentTargetSteps[i] > _currentSteps[i]); // Von AKTUELLER Position
//         _setDirection(i, dir);
//     }
// }

// // void TrajectoryExecutor::_updateSegment()
// // {
// //     if (_currentPointIdx >= _trajectory.size() - 1)
// //         return;

// //     // Aktueller Punkt = Start
// //     const TrajectoryPoint& current = _trajectory[_currentPointIdx];
// //     // Nächster Punkt = Ziel
// //     const TrajectoryPoint& next = _trajectory[_currentPointIdx + 1];

// //     // Zeit für dieses Segment berechnen
// //     uint32_t segmentDurationMs = next.timeMs - current.timeMs;
// //     if (segmentDurationMs == 0)
// //     {
// //         segmentDurationMs = 1;
// //     }

// //     for (int i = 0; i < 6; i++)
// //     {
// //         _segmentStartSteps[i]  = current.steps[i];
// //         _segmentTargetSteps[i] = next.steps[i];
// //         _segmentDelta[i]       = abs(_segmentTargetSteps[i] - _segmentStartSteps[i]);
// //     }

// //     // Max-Delta finden
// //     _maxDelta = 0;
// //     for (int i = 0; i < 6; i++)
// //     {
// //         if (_segmentDelta[i] > _maxDelta)
// //         {
// //             _maxDelta = _segmentDelta[i];
// //         }
// //     }

// //     // WICHTIG: Berechne wie viele ISR-Aufrufe für dieses Segment
// //     // Bei 50µs Timer = 20.000 Aufrufe pro Sekunde
// //     _segmentTotalSteps = (segmentDurationMs * 20000UL) / 1000UL;
// //     if (_segmentTotalSteps == 0)
// //         _segmentTotalSteps = 1;

// //     // Bresenham-Fehler initialisieren
// //     for (int i = 0; i < 6; i++)
// //     {
// //         _error[i] = _segmentTotalSteps / 2;
// //     }

// //     // // Bresenham-Fehler initialisieren
// //     // for (int i = 0; i < 6; i++)
// //     // {
// //     //     _error[i] = _maxDelta / 2;
// //     // }

// //     // Richtungen setzen
// //     for (int i = 0; i < 6; i++)
// //     {
// //         bool dir = (_segmentTargetSteps[i] > _segmentStartSteps[i]);
// //         _setDirection(i, dir);
// //     }

// //     // Sicherstellen dass _maxDelta nicht 0 ist
// //     if (_maxDelta == 0)
// //         _maxDelta = 1;
// // }

// void TrajectoryExecutor::_doBresenhamStep()
// {
//     for (int i = 0; i < 6; i++)
//     {
//         if (_currentSteps[i] == _segmentTargetSteps[i])
//             continue;

//         _error[i] += _segmentDelta[i];

//         // Klassische Bresenham-Logik mit _maxDelta
//         if (_error[i] >= _maxDelta)
//         {
//             _error[i] -= _maxDelta;
//             _pulseStep(i);

//             // Position aktualisieren
//             if (_segmentTargetSteps[i] > _currentSteps[i])
//             {
//                 _currentSteps[i]++;
//             }
//             else
//             {
//                 _currentSteps[i]--;
//             }
//         }
//     }
// }

// // void TrajectoryExecutor::_doBresenhamStep()
// // {
// //     for (int i = 0; i < 6; i++)
// //     {
// //         if (_currentSteps[i] == _segmentTargetSteps[i])
// //             continue;

// //         _error[i] += _segmentDelta[i];

// //         // GEÄNDERT: Verwende _segmentTotalSteps statt _maxDelta
// //         if (_error[i] >= _segmentTotalSteps)
// //         {
// //             _error[i] -= _segmentTotalSteps;
// //             _pulseStep(i);

// //             if (_segmentTargetSteps[i] > _currentSteps[i])
// //             {
// //                 _currentSteps[i]++;
// //             }
// //             else
// //             {
// //                 _currentSteps[i]--;
// //             }
// //         }

// //         // if (_error[i] >= _maxDelta)
// //         // {
// //         //     _error[i] -= _maxDelta;
// //         //     _pulseStep(i);

// //         //     // Position aktualisieren
// //         //     if (_segmentTargetSteps[i] > _currentSteps[i])
// //         //     {
// //         //         _currentSteps[i]++;
// //         //     }
// //         //     else
// //         //     {
// //         //         _currentSteps[i]--;
// //         //     }
// //         // }
// //     }
// // }

// void TrajectoryExecutor::_pulseStep(uint8_t motorIdx)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->stepPin;
//     digitalWriteFast(pin, HIGH);
//     delayMicroseconds(1); // 1µs Puls
//     digitalWriteFast(pin, LOW);
// }

// void TrajectoryExecutor::_setDirection(uint8_t motorIdx, bool direction)
// {
//     if (motorIdx >= _motorConfigs.size() || !_motorConfigs[motorIdx])
//         return;

//     uint8_t pin = _motorConfigs[motorIdx]->dirPin;
//     digitalWriteFast(pin, direction ? HIGH : LOW);
// }

// void TrajectoryExecutor::_staticStepTimerISR()
// {
//     if (_instance)
//     {
//         _instance->_stepTimerISR();
//     }
// }

// void TrajectoryExecutor::update()
// {
//     // Wird regelmäßig in loop() aufgerufen für Status-Updates
// }

// float TrajectoryExecutor::getProgress() const
// {
//     if (!_isRunning || _totalTimeMs == 0)
//         return 0.0f;

//     uint32_t elapsed = millis() - _trajectoryStartTime;
//     return min(100.0f, (elapsed * 100.0f) / _totalTimeMs);
// }