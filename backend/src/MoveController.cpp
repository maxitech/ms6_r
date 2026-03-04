#include "MoveController.h"

MoveController::MoveController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender)
    , _isRunning(false)
    , _accelStage(ACCEL_RAMP_UP)
    , _masterIndex(0)
    , _maxDistance(0)
    , _masterStepCounter(0)
    , _initialized(false)
    , _currentSpeed(0.0f)
    , _targetSpeed(0.0f)
    , _stepInterval(0)
    , _lastStepTime(0)
    , _stepCounter(0)
{
    // Initialize arrays
    for (int i = 0; i < 6; i++)
    {
        _error[i]          = 0;
        _totalSteps[i]     = 0;
        _stepsRemaining[i] = 0;
        _directions[i]     = true;
    }
}

void MoveController::runAllSimultaneously(float m1Pos, float m2Pos, float m3Pos, float m4Pos, float m5Pos, float m6Pos, float speedOverride)
{
    // Wenn nicht initialisiert, Setup durchführen
    if (!_initialized)
    {
        // 1. Distanzen berechnen
        long distances[6];
        distances[0] = abs(m1Pos - _motorConfigs[0]->motor->currentPosition());
        distances[1] = abs(m2Pos - _motorConfigs[1]->motor->currentPosition());
        distances[2] = abs(m3Pos - _motorConfigs[2]->motor->currentPosition());
        distances[3] = abs(m4Pos - _motorConfigs[3]->motor->currentPosition());
        distances[4] = abs(m5Pos - _motorConfigs[4]->motor->currentPosition());
        distances[5] = abs(m6Pos - _motorConfigs[5]->motor->currentPosition());

        // 2. Master-Achse finden (längste Distanz)
        _masterIndex = 0;
        _maxDistance = distances[0];
        for (int i = 1; i < 6; i++)
        {
            if (distances[i] > _maxDistance)
            {
                _maxDistance = distances[i];
                _masterIndex = i;
            }
        }

        // 3. Richtungen setzen
        _directions[0] = (m1Pos > _motorConfigs[0]->motor->currentPosition());
        _directions[1] = (m2Pos > _motorConfigs[1]->motor->currentPosition());
        _directions[2] = (m3Pos > _motorConfigs[2]->motor->currentPosition());
        _directions[3] = (m4Pos > _motorConfigs[3]->motor->currentPosition());
        _directions[4] = (m5Pos > _motorConfigs[4]->motor->currentPosition());
        _directions[5] = (m6Pos > _motorConfigs[5]->motor->currentPosition());

        // 4. Physikalische Pins setzen
        setDirectionPin(_motorConfigs[0]->dirPin, _directions[0]);
        setDirectionPin(_motorConfigs[1]->dirPin, _directions[1]);
        setDirectionPin(_motorConfigs[2]->dirPin, _directions[2]);
        setDirectionPin(_motorConfigs[3]->dirPin, _directions[3]);
        setDirectionPin(_motorConfigs[4]->dirPin, _directions[4]);
        setDirectionPin(_motorConfigs[5]->dirPin, _directions[5]);

        // 5. Bresenham initialisieren
        for (int i = 0; i < 6; i++)
        {
            _totalSteps[i]     = distances[i];
            _stepsRemaining[i] = distances[i];
            _error[i]          = _maxDistance / 2;
        }

        // 6. Geschwindigkeitsprofile initialisieren
        _currentSpeed = 100.0f; // STARTGESCHWINDIGKEIT ERHÖHEN (nicht 0!)

        // Maximalgeschwindigkeiten pro Achse definieren
        float maxSpeeds[6] = {
            MAX_SPEED_M1, MAX_SPEED_M2, MAX_SPEED_M3,
            MAX_SPEED_M4, MAX_SPEED_M5, MAX_SPEED_M6};

        // Berechne das maximale Geschwindigkeitsverhältnis
        // Finde die achsenabhängige Begrenzung
        _targetSpeed = maxSpeeds[_masterIndex]; // Start mit Master-Maximalgeschwindigkeit

        for (int i = 0; i < 6; i++)
        {
            if (i == _masterIndex || _totalSteps[i] == 0)
                continue;

            // Wenn eine Slave-Achse mehr Schritte pro Zeiteinheit machen müsste
            // als ihre Maximalgeschwindigkeit erlaubt
            float ratio              = (float)_totalSteps[i] / _totalSteps[_masterIndex];
            float requiredSlaveSpeed = ratio * _targetSpeed;

            if (requiredSlaveSpeed > maxSpeeds[i])
            {
                // Master-Geschwindigkeit reduzieren, damit Slave nicht zu schnell wird
                float limitedMasterSpeed = maxSpeeds[i] / ratio;
                if (limitedMasterSpeed < _targetSpeed)
                {
                    _targetSpeed = limitedMasterSpeed;
                }
            }
        }

        // Für kurze Bewegungen: Geschwindigkeit anpassen
        if (_maxDistance < 100)
        {
            _targetSpeed = min(_targetSpeed, 500.0f); // Sehr langsam für minimale Distanzen
        }
        else if (_maxDistance < 300)
        {
            _targetSpeed = min(_targetSpeed, 1000.0f);
        }
        else if (_maxDistance < 600)
        {
            _targetSpeed = min(_targetSpeed, 2000.0f);
        }
        else if (_maxDistance < 1200)
        {
            _targetSpeed = min(_targetSpeed, 5000.0f);
        }
        // Für >1200: Kein zusätzliches Limit, nur Achsenlimits

        _targetSpeed *= speedOverride;

        // Startintervall berechnen (basierend auf Startgeschwindigkeit)
        if (_currentSpeed > 0)
        {
            _stepInterval = 1000000.0f / _currentSpeed;
        }
        else
        {
            _stepInterval = 10000; // 100 Hz als sicherer Startwert
        }

        _stepCounter = 0;
        _accelStage  = ACCEL_RAMP_UP;

        // AccelStepper-Ziele setzen (für Positionsverfolgung)
        _motorConfigs[0]->motor->moveTo(m1Pos);
        _motorConfigs[1]->motor->moveTo(m2Pos);
        _motorConfigs[2]->motor->moveTo(m3Pos);
        _motorConfigs[3]->motor->moveTo(m4Pos);
        _motorConfigs[4]->motor->moveTo(m5Pos);
        _motorConfigs[5]->motor->moveTo(m6Pos);

        _initialized  = true;
        _isRunning    = true;
        _lastStepTime = micros();

        return;
    }

    // Hauptbewegungsschleife
    if (_isRunning)
    {
        unsigned long currentTime       = micros();
        unsigned long timeSinceLastStep = currentTime - _lastStepTime;

        // Prüfen, ob es Zeit für einen Schritt ist
        if (timeSinceLastStep >= _stepInterval && _stepsRemaining[_masterIndex] > 0)
        {
            // 1. Master-Achse einen Schritt machen
            pulseStepPin(_motorConfigs[_masterIndex]->stepPin);

            // 2. AccelStepper-Position aktualisieren
            long newPos = _motorConfigs[_masterIndex]->motor->currentPosition() +
                          (_directions[_masterIndex] ? 1 : -1);
            _motorConfigs[_masterIndex]->motor->setCurrentPosition(newPos);

            _stepsRemaining[_masterIndex]--;
            _stepCounter++;

            // 3. Bresenham für Slave-Achsen
            for (int i = 0; i < 6; i++)
            {
                if (i == _masterIndex || _stepsRemaining[i] <= 0)
                {
                    continue;
                }

                // Bresenham-Algorithmus
                _error[i] += _totalSteps[i];
                if (_error[i] >= _maxDistance)
                {
                    _error[i] -= _maxDistance;

                    // Slave-Achse einen Schritt machen
                    pulseStepPin(_motorConfigs[i]->stepPin);

                    // AccelStepper-Position aktualisieren
                    newPos = _motorConfigs[i]->motor->currentPosition() +
                             (_directions[i] ? 1 : -1);
                    _motorConfigs[i]->motor->setCurrentPosition(newPos);

                    _stepsRemaining[i]--;
                }
            }

            _lastStepTime = currentTime;

            // 4. Beschleunigungs-/Verzögerungsprofil berechnen
            updateSpeedProfile();

            // 5. Neues Schrittintervall berechnen
            if (_currentSpeed > 0)
            {
                _stepInterval = 1000000.0f / _currentSpeed;
            }
            else
            {
                _stepInterval = 0xFFFFFFFF;
            }
        }

        // Prüfen, ob alle Achsen fertig sind
        bool allDone = true;
        for (int i = 0; i < 6; i++)
        {
            if (_stepsRemaining[i] > 0)
            {
                allDone = false;
                break;
            }
        }

        if (allDone)
        {
            // Bewegung abgeschlossen
            _isRunning   = false;
            _initialized = false;
            _accelStage  = ACCEL_RAMP_UP;

            // Finale Positionen synchronisieren
            _rbtDtaSender.sendMotorPosInSteps(_motorConfigs);
        }
    }
}

void MoveController::updateSpeedProfile()
{
    float remainingMasterSteps = _stepsRemaining[_masterIndex];

    // Bestimme Beschleunigung basierend auf Distanz
    float ACCEL;
    if (_maxDistance < 100)
        ACCEL = 800.0f;
    else if (_maxDistance < 300)
        ACCEL = 1500.0f;
    else if (_maxDistance < 600)
        ACCEL = 3000.0f;
    else if (_maxDistance < 1200)
        ACCEL = 6000.0f;
    else
        ACCEL = MAX_Acc;

    float dt = _stepInterval / 1000000.0f;

    // Für sehr kurze Distanzen: Direktes Dreiecksprofil
    if (_maxDistance < 150)
    {
        float midPoint = _maxDistance * 0.6f; // 60% für Beschleunigung
        if (remainingMasterSteps > midPoint)
        {
            // Beschleunigen
            _currentSpeed += ACCEL * dt;
            float maxSpeed = min(_targetSpeed, sqrt(ACCEL * _maxDistance * 0.8f));
            if (_currentSpeed > maxSpeed)
                _currentSpeed = maxSpeed;
        }
        else
        {
            // Verzögern
            _currentSpeed -= ACCEL * dt;
            if (_currentSpeed < 80.0f)
                _currentSpeed = 80.0f;
        }
        return;
    }

    // Für normale Distanzen
    switch (_accelStage)
    {
    case ACCEL_RAMP_UP:
    {
        _currentSpeed += ACCEL * dt;

        // Erreichte Zielgeschwindigkeit?
        if (_currentSpeed >= _targetSpeed * 0.95f)
        {
            _currentSpeed = _targetSpeed;
            // Für mittlere Distanzen: Kein Cruise
            if (_maxDistance < 800)
                _accelStage = ACCEL_RAMP_DOWN;
            else
                _accelStage = ACCEL_CRUISE;
        }

        // Brauchen wir sofort zu bremsen?
        float brakeDist = (_currentSpeed * _currentSpeed) / (2 * ACCEL);
        if (remainingMasterSteps <= brakeDist * 1.8f) // Mehr Reserve für kurze
            _accelStage = ACCEL_RAMP_DOWN;
        break;
    }

    case ACCEL_CRUISE:
    {
        _currentSpeed = _targetSpeed;

        // Beginne Bremsen
        float brakeDist = (_currentSpeed * _currentSpeed) / (2 * ACCEL);
        if (remainingMasterSteps <= brakeDist * 1.3f)
            _accelStage = ACCEL_RAMP_DOWN;
        break;
    }
    case ACCEL_RAMP_DOWN:
    {
        // Sanfte Verzögerung basierend auf verbleibender Distanz
        float targetSpeed = 0.0f;

        if (remainingMasterSteps > 100)
        {
            // Phase 1: Progressive Verzögerung
            targetSpeed = sqrt(2 * ACCEL * remainingMasterSteps * 0.8f);
        }
        else if (remainingMasterSteps > 30)
        {
            // Phase 2: Lineare Reduktion
            targetSpeed = 800.0f;
        }
        else if (remainingMasterSteps > 10)
        {
            // Phase 3: Langsame Annäherung
            targetSpeed = 400.0f;
        }
        else
        {
            // Phase 4: Letzte Schritte
            targetSpeed = 150.0f;
        }

        // Sanft auf Zielgeschwindigkeit anpassen
        if (_currentSpeed > targetSpeed + 100.0f)
        {
            _currentSpeed -= ACCEL * dt * 1.2f;
        }
        else if (_currentSpeed > targetSpeed)
        {
            _currentSpeed -= ACCEL * dt * 0.5f;
        }

        if (_currentSpeed < 50.0f)
            _currentSpeed = 50.0f;
        break;
    }
    }
}

// void MoveController::updateSpeedProfile()
// {
//     // Maximale Werte für Beschleunigung und Jerk
//     float MAX_ACCELERATION = MAX_Acc;
//     float MAX_JERK         = MAX_Acc * 10.0f; // Jerk ist typisch 5-20x die Beschleunigung

//     if (_maxDistance < 500)
//     {
//         MAX_ACCELERATION *= 0.2f; // Nicht 0.5, sondern 0.2!
//         MAX_JERK *= 0.2f;
//     }
//     else if (_maxDistance < 2000)
//     {
//         MAX_ACCELERATION *= 0.4f; // Gestaffelt
//         MAX_JERK *= 0.4f;
//     }

//     if (_targetSpeed > 20000.0f)
//     {
//         MAX_ACCELERATION = 20000.0f;
//         MAX_JERK         = 200000.0f;
//     }

//     float       remainingMasterSteps = _stepsRemaining[_masterIndex];
//     float       dt                   = _stepInterval / 1000000.0f; // Zeitschritt in Sekunden
//     const float MIN_END_SPEED        = 50.0f;

//     // Berechne benötigte Bremsdistanz mit S-Kurve
//     // Grobe Näherung: S-Kurve braucht etwa 1.5x mehr Platz als lineare Rampe
//     // Je höher, desto früher fängt er an zu bremsen
//     float decelDistance = (_currentSpeed * _currentSpeed) / (2 * MAX_ACCELERATION) * 2.2f;

//     switch (_accelStage)
//     {
//     case ACCEL_RAMP_UP:
//     {
//         // S-Kurve Beschleunigung: Jerk begrenzt die Änderung der Beschleunigung
//         if (_currentAcceleration < MAX_ACCELERATION)
//         {
//             _currentAcceleration += MAX_JERK * dt;
//             if (_currentAcceleration > MAX_ACCELERATION)
//             {
//                 _currentAcceleration = MAX_ACCELERATION;
//             }
//         }

//         // Geschwindigkeit erhöhen mit aktueller Beschleunigung
//         _currentSpeed += _currentAcceleration * dt;

//         // Prüfe ob Zielgeschwindigkeit erreicht
//         if (_currentSpeed >= _targetSpeed)
//         {
//             _currentSpeed = _targetSpeed;
//             _accelStage   = ACCEL_CRUISE;
//         }

//         // Prüfe ob Bremsen eingeleitet werden muss
//         if (remainingMasterSteps <= decelDistance * 1.2f)
//         {
//             _accelStage = ACCEL_RAMP_DOWN;
//         }
//         break;
//     }

//     case ACCEL_CRUISE:
//     {
//         // Halte Beschleunigung bei 0 während Cruise
//         _currentAcceleration = 0;

//         // Berechne Bremsdistanz neu
//         decelDistance = (_currentSpeed * _currentSpeed) / (2 * MAX_ACCELERATION) * 2.2f;

//         // Je höher, desto früher startet die Bremsphase
//         if (remainingMasterSteps <= decelDistance * 1.4f)
//         {
//             _accelStage = ACCEL_RAMP_DOWN;
//         }
//         break;
//     }

//     case ACCEL_RAMP_DOWN:
//     {
//         // Berechne ideale Zielgeschwindigkeit für verbleibende Strecke
//         // v² = 2 * a * s, aber mit Puffer für S-Kurve
//         // Je niedriger, desto langsamer wird er am Ende sein
//         float idealSpeed = sqrt(2 * MAX_ACCELERATION * remainingMasterSteps * 0.65f);
//         idealSpeed       = min(idealSpeed, _currentSpeed);

//         // Bestimme ob wir verzögern oder halten müssen
//         if (_currentSpeed > idealSpeed + 50.0f) // Kleine Hysterese
//         {
//             // Verzögern mit S-Kurve: Jerk reduziert Beschleunigung
//             if (_currentAcceleration > -MAX_ACCELERATION)
//             {
//                 _currentAcceleration -= MAX_JERK * dt;
//                 if (_currentAcceleration < -MAX_ACCELERATION)
//                 {
//                     _currentAcceleration = -MAX_ACCELERATION;
//                 }
//             }
//         }
//         else if (_currentSpeed < idealSpeed - 50.0f)
//         {
//             // Zu langsam - sanft zurück beschleunigen (sollte selten vorkommen)
//             if (_currentAcceleration < 0)
//             {
//                 _currentAcceleration += MAX_JERK * dt * 2.0f; // Schneller zurück
//                 if (_currentAcceleration > 0)
//                 {
//                     _currentAcceleration = 0;
//                 }
//             }
//         }
//         else
//         {
//             // Nahe der idealen Geschwindigkeit - Beschleunigung sanft gegen 0
//             if (_currentAcceleration < 0)
//             {
//                 _currentAcceleration += MAX_JERK * dt * 0.5f;
//                 if (_currentAcceleration > 0)
//                 {
//                     _currentAcceleration = 0;
//                 }
//             }
//         }

//         // Geschwindigkeit aktualisieren
//         _currentSpeed += _currentAcceleration * dt;

//         // Mindestgeschwindigkeit für letzte Steps
//         if (remainingMasterSteps < 100)
//         {
//             float minSpeed = max(MIN_END_SPEED, idealSpeed * 0.8f);
//             if (_currentSpeed < minSpeed)
//             {
//                 _currentSpeed        = minSpeed;
//                 _currentAcceleration = 0; // Keine weitere Verzögerung
//             }
//         }

//         // Absolute Mindestgeschwindigkeit
//         if (_currentSpeed < MIN_END_SPEED)
//         {
//             _currentSpeed        = MIN_END_SPEED;
//             _currentAcceleration = 0;
//         }

//         break;
//     }
//     }
// }

// void MoveController::updateSpeedProfile()
// {
//     // Dynamische Beschleunigung basierend auf Zielgeschwindigkeit
//     float ACCELERATION = MAX_Acc;

//     if (_targetSpeed > 20000.0f)
//     {
//         ACCELERATION = 20000.0f;
//     }

//     float remainingMasterSteps = _stepsRemaining[_masterIndex];
//     float decelDistance        = 0; // Variable außerhalb des switch deklarieren

//     switch (_accelStage)
//     {
//     case ACCEL_RAMP_UP:
//         _currentSpeed += ACCELERATION * (_stepInterval / 1000000.0f);

//         if (_currentSpeed >= _targetSpeed)
//         {
//             _currentSpeed = _targetSpeed;
//             _accelStage   = ACCEL_CRUISE;
//         }

//         decelDistance = (_currentSpeed * _currentSpeed) / (2 * ACCELERATION);
//         if (remainingMasterSteps <= decelDistance * 1.2f)
//         {
//             _accelStage = ACCEL_RAMP_DOWN;
//         }
//         break;

//     case ACCEL_CRUISE:
//         decelDistance = (_currentSpeed * _currentSpeed) / (2 * ACCELERATION);
//         if (remainingMasterSteps <= decelDistance * 1.1f)
//         {
//             _accelStage = ACCEL_RAMP_DOWN;
//         }
//         break;

//     case ACCEL_RAMP_DOWN:
//         _currentSpeed -= ACCELERATION * (_stepInterval / 1000000.0f);

//         if (_currentSpeed < 50.0f)
//         {
//             _currentSpeed = 50.0f;
//             if (remainingMasterSteps < 10)
//             {
//                 _currentSpeed = 500.0f;
//             }
//         }
//         break;
//     }
// }

// void MoveController::runAllSimultaneously(float m1Pos, float m2Pos, float m3Pos, float m4Pos, float m5Pos, float m6Pos)
// {
//     // Wenn nicht initialisiert, Setup durchführen
//     if (!_initialized)
//     {
//         // 1. Distanzen berechnen
//         long distances[6];
//         distances[0] = abs(m1Pos - _motorConfigs[0]->motor->currentPosition());
//         distances[1] = abs(m2Pos - _motorConfigs[1]->motor->currentPosition());
//         distances[2] = abs(m3Pos - _motorConfigs[2]->motor->currentPosition());
//         distances[3] = abs(m4Pos - _motorConfigs[3]->motor->currentPosition());
//         distances[4] = abs(m5Pos - _motorConfigs[4]->motor->currentPosition());
//         distances[5] = abs(m6Pos - _motorConfigs[5]->motor->currentPosition());

//         // 2. Master-Achse finden (längste Distanz)
//         _masterIndex = 0;
//         _maxDistance = distances[0];
//         for (int i = 1; i < 6; i++)
//         {
//             if (distances[i] > _maxDistance)
//             {
//                 _maxDistance = distances[i];
//                 _masterIndex = i;
//             }
//         }

//         // 3. Richtungen setzen
//         _directions[0] = (m1Pos > _motorConfigs[0]->motor->currentPosition());
//         _directions[1] = (m2Pos > _motorConfigs[1]->motor->currentPosition());
//         _directions[2] = (m3Pos > _motorConfigs[2]->motor->currentPosition());
//         _directions[3] = (m4Pos > _motorConfigs[3]->motor->currentPosition());
//         _directions[4] = (m5Pos > _motorConfigs[4]->motor->currentPosition());
//         _directions[5] = (m6Pos > _motorConfigs[5]->motor->currentPosition());

//         // 4. Physikalische Pins setzen
//         setDirectionPin(_motorConfigs[0]->dirPin, _directions[0]);
//         setDirectionPin(_motorConfigs[1]->dirPin, _directions[1]);
//         setDirectionPin(_motorConfigs[2]->dirPin, _directions[2]);
//         setDirectionPin(_motorConfigs[3]->dirPin, _directions[3]);
//         setDirectionPin(_motorConfigs[4]->dirPin, _directions[4]);
//         setDirectionPin(_motorConfigs[5]->dirPin, _directions[5]);

//         // 5. Bresenham initialisieren
//         for (int i = 0; i < 6; i++)
//         {
//             _totalSteps[i]     = distances[i];
//             _stepsRemaining[i] = distances[i];
//             _error[i]          = _maxDistance / 2;
//         }

//         // 6. Geschwindigkeitsprofile initialisieren
//         _currentSpeed = 0.0f;

//         // Maximalgeschwindigkeit basierend auf Master-Achse
//         switch (_masterIndex)
//         {
//         case 0:
//             _targetSpeed = MAX_SPEED_M1;
//             break;
//         case 1:
//             _targetSpeed = MAX_SPEED_M2;
//             break;
//         case 2:
//             _targetSpeed = MAX_SPEED_M3;
//             break;
//         case 3:
//             _targetSpeed = MAX_SPEED_M4;
//             break;
//         case 4:
//             _targetSpeed = MAX_SPEED_M5;
//             break;
//         case 5:
//             _targetSpeed = MAX_SPEED_M6;
//             break;
//         }

//         // Für kurze Bewegungen: Geschwindigkeit anpassen
//         if (_maxDistance < 500)
//         {
//             _targetSpeed = min(_targetSpeed, 1000.0f);
//         }

//         if (_currentSpeed > 0)
//         {
//             _stepInterval = 1000000.0f / _currentSpeed;
//         }
//         else
//         {
//             _stepInterval = 1000000; // Start with 1 second interval
//         }

//         _stepCounter = 0;
//         _accelStage  = ACCEL_RAMP_UP;

//         // AccelStepper-Ziele setzen (für Positionsverfolgung)
//         _motorConfigs[0]->motor->moveTo(m1Pos);
//         _motorConfigs[1]->motor->moveTo(m2Pos);
//         _motorConfigs[2]->motor->moveTo(m3Pos);
//         _motorConfigs[3]->motor->moveTo(m4Pos);
//         _motorConfigs[4]->motor->moveTo(m5Pos);
//         _motorConfigs[5]->motor->moveTo(m6Pos);

//         _initialized  = true;
//         _isRunning    = true;
//         _lastStepTime = micros();

//         return;
//     }

//     // Hauptbewegungsschleife
//     if (_isRunning)
//     {
//         unsigned long currentTime       = micros();
//         unsigned long timeSinceLastStep = currentTime - _lastStepTime;

//         // Prüfen, ob es Zeit für einen Schritt ist
//         if (timeSinceLastStep >= _stepInterval && _stepsRemaining[_masterIndex] > 0)
//         {
//             // 1. Master-Achse einen Schritt machen
//             pulseStepPin(_motorConfigs[_masterIndex]->stepPin);

//             // 2. AccelStepper-Position aktualisieren
//             long newPos = _motorConfigs[_masterIndex]->motor->currentPosition() +
//                           (_directions[_masterIndex] ? 1 : -1);
//             _motorConfigs[_masterIndex]->motor->setCurrentPosition(newPos);

//             _stepsRemaining[_masterIndex]--;
//             _stepCounter++;

//             // 3. Bresenham für Slave-Achsen
//             for (int i = 0; i < 6; i++)
//             {
//                 if (i == _masterIndex || _stepsRemaining[i] <= 0)
//                 {
//                     continue;
//                 }

//                 // Bresenham-Algorithmus
//                 _error[i] += _totalSteps[i];
//                 if (_error[i] >= _maxDistance)
//                 {
//                     _error[i] -= _maxDistance;

//                     // Slave-Achse einen Schritt machen
//                     pulseStepPin(_motorConfigs[i]->stepPin);

//                     // AccelStepper-Position aktualisieren
//                     newPos = _motorConfigs[i]->motor->currentPosition() +
//                              (_directions[i] ? 1 : -1);
//                     _motorConfigs[i]->motor->setCurrentPosition(newPos);

//                     _stepsRemaining[i]--;
//                 }
//             }

//             _lastStepTime = currentTime;

//             // 4. Beschleunigungs-/Verzögerungsprofil berechnen
//             updateSpeedProfile();

//             // 5. Neues Schrittintervall berechnen
//             if (_currentSpeed > 0)
//             {
//                 _stepInterval = 1000000.0f / _currentSpeed;
//             }
//             else
//             {
//                 _stepInterval = 0xFFFFFFFF;
//             }
//         }

//         // Prüfen, ob alle Achsen fertig sind
//         bool allDone = true;
//         for (int i = 0; i < 6; i++)
//         {
//             if (_stepsRemaining[i] > 0)
//             {
//                 allDone = false;
//                 break;
//             }
//         }

//         if (allDone)
//         {
//             // Bewegung abgeschlossen
//             _isRunning   = false;
//             _initialized = false;
//             _accelStage  = ACCEL_RAMP_UP;

//             // Finale Positionen synchronisieren
//             _rbtDtaSender.sendMotorPosInSteps(_motorConfigs);
//         }
//     }
// }

// void MoveController::updateSpeedProfile()
// {
//     static const float ACCELERATION = 1000.0f;

//     float remainingMasterSteps = _stepsRemaining[_masterIndex];

//     switch (_accelStage)
//     {
//     case ACCEL_RAMP_UP:
//         _currentSpeed += ACCELERATION * (_stepInterval / 1000000.0f);

//         if (_currentSpeed >= _targetSpeed)
//         {
//             _currentSpeed = _targetSpeed;
//             _accelStage   = ACCEL_CRUISE;
//         }

//         if (remainingMasterSteps < (_currentSpeed * _currentSpeed) / (2 * ACCELERATION))
//         {
//             _accelStage = ACCEL_RAMP_DOWN;
//         }
//         break;

//     case ACCEL_CRUISE:
//         if (remainingMasterSteps < (_currentSpeed * _currentSpeed) / (2 * ACCELERATION))
//         {
//             _accelStage = ACCEL_RAMP_DOWN;
//         }
//         break;

//     case ACCEL_RAMP_DOWN:
//         _currentSpeed -= ACCELERATION * (_stepInterval / 1000000.0f);
//         if (_currentSpeed < 10.0f)
//         {
//             _currentSpeed = 10.0f;
//         }
//         break;
//     }
// }

bool MoveController::motorsAtTarget()
{
    return !_isRunning;
}

void MoveController::emergencyStop()
{
    _isRunning   = false;
    _initialized = false;
    _accelStage  = ACCEL_RAMP_UP;

    for (auto* cfg : _motorConfigs)
    {
        if (cfg->motor != nullptr)
        {
            cfg->motor->setSpeed(0);
            cfg->motor->stop();
        }
    }
}

void MoveController::reset()
{
    emergencyStop();

    _masterIndex         = 0;
    _maxDistance         = 0;
    _masterStepCounter   = 0;
    _currentSpeed        = 0.0f;
    _targetSpeed         = 0.0f;
    _stepInterval        = 0;
    _lastStepTime        = 0;
    _stepCounter         = 0;
    _currentAcceleration = 0.0f;

    for (int i = 0; i < 6; i++)
    {
        _error[i]          = 0;
        _totalSteps[i]     = 0;
        _stepsRemaining[i] = 0;
        _directions[i]     = true;
    }
}