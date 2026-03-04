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

void MoveController::runAllSimultaneously(float m1Pos, float m2Pos, float m3Pos, float m4Pos, float m5Pos, float m6Pos)
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
        _currentSpeed = 0.0f;

        // Maximalgeschwindigkeit basierend auf Master-Achse
        switch (_masterIndex)
        {
        case 0:
            _targetSpeed = MAX_SPEED_M1;
            break;
        case 1:
            _targetSpeed = MAX_SPEED_M2;
            break;
        case 2:
            _targetSpeed = MAX_SPEED_M3;
            break;
        case 3:
            _targetSpeed = MAX_SPEED_M4;
            break;
        case 4:
            _targetSpeed = MAX_SPEED_M5;
            break;
        case 5:
            _targetSpeed = MAX_SPEED_M6;
            break;
        }

        // Für kurze Bewegungen: Geschwindigkeit anpassen
        if (_maxDistance < 500)
        {
            _targetSpeed = min(_targetSpeed, 1000.0f);
        }

        if (_currentSpeed > 0)
        {
            _stepInterval = 1000000.0f / _currentSpeed;
        }
        else
        {
            _stepInterval = 1000000; // Start with 1 second interval
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
    static const float ACCELERATION = 1000.0f;

    float remainingMasterSteps = _stepsRemaining[_masterIndex];

    switch (_accelStage)
    {
    case ACCEL_RAMP_UP:
        _currentSpeed += ACCELERATION * (_stepInterval / 1000000.0f);

        if (_currentSpeed >= _targetSpeed)
        {
            _currentSpeed = _targetSpeed;
            _accelStage   = ACCEL_CRUISE;
        }

        if (remainingMasterSteps < (_currentSpeed * _currentSpeed) / (2 * ACCELERATION))
        {
            _accelStage = ACCEL_RAMP_DOWN;
        }
        break;

    case ACCEL_CRUISE:
        if (remainingMasterSteps < (_currentSpeed * _currentSpeed) / (2 * ACCELERATION))
        {
            _accelStage = ACCEL_RAMP_DOWN;
        }
        break;

    case ACCEL_RAMP_DOWN:
        _currentSpeed -= ACCELERATION * (_stepInterval / 1000000.0f);
        if (_currentSpeed < 10.0f)
        {
            _currentSpeed = 10.0f;
        }
        break;
    }
}

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

    _masterIndex       = 0;
    _maxDistance       = 0;
    _masterStepCounter = 0;
    _currentSpeed      = 0.0f;
    _targetSpeed       = 0.0f;
    _stepInterval      = 0;
    _lastStepTime      = 0;
    _stepCounter       = 0;

    for (int i = 0; i < 6; i++)
    {
        _error[i]          = 0;
        _totalSteps[i]     = 0;
        _stepsRemaining[i] = 0;
        _directions[i]     = true;
    }
}