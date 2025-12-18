#include "MoveController.h"

MoveController::MoveController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender) {};

void MoveController::runAllSimultaneously(float m1Pos, float m2Pos, float m3Pos, float m4Pos, float m5Pos, float m6Pos) // AccelStepper with slaved acceleration
{

    static int stage = 0;

    if (stage == 0)
    {
        // Step 1: Who has the farthest distance to travel?

        motor1->setMaxSpeed(MAX_SPEED_M1);
        motor2->setMaxSpeed(MAX_SPEED_M2);
        motor3->setMaxSpeed(MAX_SPEED_M3);
        motor4->setMaxSpeed(MAX_SPEED_M4);
        motor5->setMaxSpeed(MAX_SPEED_M5);
        motor6->setMaxSpeed(MAX_SPEED_M6);
        // resetAcceleration(); //it's a function I created that simply sets the max acceleration of each motor to some default value
        motor1->setAcceleration(MAX_Acc);
        motor2->setAcceleration(MAX_Acc);
        motor3->setAcceleration(MAX_Acc);
        motor4->setAcceleration(MAX_Acc);
        motor5->setAcceleration(MAX_Acc);
        motor6->setAcceleration(MAX_Acc);

        longestD2G = 0;
        m1D2G      = abs(m1Pos - motor1->currentPosition());
        m2D2G      = abs(m2Pos - motor2->currentPosition());
        m3D2G      = abs(m3Pos - motor3->currentPosition());
        m4D2G      = abs(m4Pos - motor4->currentPosition());
        m5D2G      = abs(m5Pos - motor5->currentPosition());
        m6D2G      = abs(m6Pos - motor6->currentPosition());

        if (m1D2G > longestD2G)
        {
            longestD2G   = m1D2G;
            longestMotor = "m1";
        }

        if (m2D2G > longestD2G)
        {
            longestD2G   = m2D2G;
            longestMotor = "m2";
        }

        if (m3D2G > longestD2G)
        {
            longestD2G   = m3D2G;
            longestMotor = "m3";
        }

        if (m4D2G > longestD2G)
        {
            longestD2G   = m4D2G;
            longestMotor = "m4";
        }

        if (m5D2G > longestD2G)
        {
            longestD2G   = m5D2G;
            longestMotor = "m5";
        }

        if (m6D2G > longestD2G)
        {
            longestD2G   = m6D2G;
            longestMotor = "m6";
        }

        // Step 2: Assign a step delay to each motor (e.g. after how many steps of the master motor should the slave motor be pulsed?)

        if (m1D2G > 0)
        {
            m1StepDelay = longestD2G / m1D2G;
        }
        else
        {
            m1StepDelay = 0;
        }

        if (m2D2G > 0)
        {
            m2StepDelay = longestD2G / m2D2G;
        }
        else
        {
            m2StepDelay = 0;
        }

        if (m3D2G > 0)
        {
            m3StepDelay = longestD2G / m3D2G;
        }
        else
        {
            m3StepDelay = 0;
        }

        if (m4D2G > 0)
        {
            m4StepDelay = longestD2G / m4D2G;
        }
        else
        {
            m4StepDelay = 0;
        }

        if (m5D2G > 0)
        {
            m5StepDelay = longestD2G / m5D2G;
        }
        else
        {
            m5StepDelay = 0;
        }

        if (m6D2G > 0)
        {
            m6StepDelay = longestD2G / m6D2G;
        }
        else
        {
            m6StepDelay = 0;
        }

        // Step 3: Set the initial stepcounter values
        stepCounter   = 0;
        m1StepCounter = m1StepDelay - 1;
        m2StepCounter = m2StepDelay - 1;
        m3StepCounter = m3StepDelay - 1;
        m4StepCounter = m4StepDelay - 1;
        m5StepCounter = m5StepDelay - 1;
        m6StepCounter = m6StepDelay - 1;

        // Step 4:   Set the motor travel moves and other values depending on which motor is the longest motor
        if (longestMotor == "m1")
        {

            motor1->moveTo(m1Pos);
            lastD2G = motor1->distanceToGo();

            // might need to add these so it knows there's a target position
            motor2->moveTo(m2Pos);
            motor3->moveTo(m3Pos);
            motor4->moveTo(m4Pos);
            motor5->moveTo(m5Pos);
            motor6->moveTo(m6Pos);
        }
        if (longestMotor == "m2")
        {

            motor2->moveTo(m2Pos);
            lastD2G = motor2->distanceToGo();

            // might need to add these so it knows there's a target position
            motor1->moveTo(m1Pos);
            motor3->moveTo(m3Pos);
            motor4->moveTo(m4Pos);
            motor5->moveTo(m5Pos);
            motor6->moveTo(m6Pos);
        }
        else if (longestMotor == "m3")
        {

            motor3->moveTo(m3Pos);
            lastD2G = motor3->distanceToGo();

            // might need to add these so it knows there's a target position
            motor1->moveTo(m1Pos);
            motor2->moveTo(m2Pos);
            motor4->moveTo(m4Pos);
            motor5->moveTo(m5Pos);
            motor6->moveTo(m6Pos);
        }
        else if (longestMotor == "m4")
        {

            motor4->moveTo(m4Pos);
            lastD2G = motor4->distanceToGo();

            // might need to add these so it knows there's a target position
            motor1->moveTo(m1Pos);
            motor2->moveTo(m2Pos);
            motor3->moveTo(m3Pos);
            motor5->moveTo(m5Pos);
            motor6->moveTo(m6Pos);
        }
        else if (longestMotor == "m5")
        {

            motor5->moveTo(m5Pos);
            lastD2G = motor5->distanceToGo();

            // might need to add these so it knows there's a target position
            motor1->moveTo(m1Pos);
            motor2->moveTo(m2Pos);
            motor3->moveTo(m3Pos);
            motor4->moveTo(m4Pos);
            motor6->moveTo(m6Pos);
        }
        else if (longestMotor == "m6")
        {

            motor6->moveTo(m6Pos);
            lastD2G = motor6->distanceToGo();

            // might need to add these so it knows there's a target position
            motor1->moveTo(m1Pos);
            motor2->moveTo(m2Pos);
            motor3->moveTo(m3Pos);
            motor4->moveTo(m4Pos);
            motor5->moveTo(m5Pos);
        }

        // Step 5: Set the direction of each motor
        if ((m1Pos - motor1->currentPosition()) < 0) // Negative means you are too far ahead, need to go backwards and direction will decrease
        {
            // Negative (Left) Direction CCW
            digitalWrite(DIR1, LOW); // set to counter clockwise
            motorM1Dir = -1;         // decrement position
        }
        else
        {
            digitalWrite(DIR1, HIGH); // set to clockwise
            motorM1Dir = 1;           // increment position
        }

        if ((m2Pos - motor2->currentPosition()) < 0) // Negative means you are too far ahead, need to go backwards and direction will decrease
        {
            // Negative (Left) Direction CCW
            digitalWrite(DIR2, LOW); // set to counter clockwise
            motorM2Dir = -1;         // decrement position
        }
        else
        {
            digitalWrite(DIR2, HIGH); // set to clockwise
            motorM2Dir = 1;           // increment position
        }

        if ((m3Pos - motor3->currentPosition()) < 0) // Negative means you are too far ahead, need to go backwards and direction will decrease
        {
            // Clockwise
            digitalWrite(DIR3, HIGH);
            motorM3Dir = -1; // decrement Y position
        }
        else
        {
            digitalWrite(DIR3, LOW); // opposite of above
            motorM3Dir = 1;          // increment position tracking location
        }

        if ((m4Pos - motor4->currentPosition()) < 0) // Negative means you are too high up, want to go down and decrement position
        {
            // Clockwise
            digitalWrite(DIR4, HIGH); // clockwise
            motorM4Dir = -1;          // decrement position
        }
        else
        {
            digitalWrite(DIR4, LOW); // opposite of above
            motorM4Dir = 1;
        }

        if ((m5Pos - motor5->currentPosition()) < 0) // Negative means you are too far ahead, need to go backwards and direction will decrease
        {
            // CCW
            digitalWrite(DIR5, LOW); // set to counter clockwise
            motorM5Dir = -1;         // decrement position
        }
        else
        {
            digitalWrite(DIR5, HIGH); // set to clockwise
            motorM5Dir = 1;           // increment position
        }

        if ((m6Pos - motor6->currentPosition()) < 0) // Negative means you are too far ahead, need to go backwards and direction will decrease
        {
            // CCW
            digitalWrite(DIR6, LOW); // set to counter clockwise
            motorM6Dir = -1;         // decrement position
        }
        else
        {
            digitalWrite(DIR6, HIGH); // set to clockwise
            motorM6Dir = 1;           // increment position
        }
        stage++;
    }

    if (stage == 1)
    {
        // Run the primary motor, and step the other motors in proportion to the primary (longest) motor
        if (longestMotor == "m1")
        {
            if (lastD2G != motor1->distanceToGo()) // if there has been a change in how much distance the motor has left to travel....then update the step counter so all the slave motors know when to move
            {
                stepCounter += m1StepDelay;
                lastD2G = motor1->distanceToGo();
            }

            motor1->run(); // would be better to have a run safely command in here specific to the motor where it checks for endstops/obstructions instead of blindly running

            coordinatedM2Move();
            coordinatedM3Move();
            coordinatedM4Move();
            coordinatedM5Move();
            coordinatedM6Move();
        }
        else if (longestMotor == "m2")
        {
            if (lastD2G != motor2->distanceToGo())
            {
                stepCounter += m2StepDelay;
                lastD2G = motor2->distanceToGo();
            }

            motor2->run();

            coordinatedM1Move();
            coordinatedM3Move();
            coordinatedM4Move();
            coordinatedM5Move();
            coordinatedM6Move();
        }
        else if (longestMotor == "m3")
        {
            if (lastD2G != motor3->distanceToGo())
            {
                stepCounter += m3StepDelay;
                lastD2G = motor3->distanceToGo();
            }

            motor3->run();

            coordinatedM1Move();
            coordinatedM2Move();
            coordinatedM4Move();
            coordinatedM5Move();
            coordinatedM6Move();
        }
        else if (longestMotor == "m4")
        {
            if (lastD2G != motor4->distanceToGo())
            {
                stepCounter += m4StepDelay;
                lastD2G = motor4->distanceToGo();
            }

            motor4->run();

            coordinatedM1Move();
            coordinatedM2Move();
            coordinatedM3Move();
            coordinatedM5Move();
            coordinatedM6Move();
        }
        else if (longestMotor == "m5")
        {
            if (lastD2G != motor5->distanceToGo())
            {
                stepCounter += m5StepDelay;
                lastD2G = motor5->distanceToGo();
            }

            motor5->run();

            coordinatedM1Move();
            coordinatedM2Move();
            coordinatedM3Move();
            coordinatedM4Move();
            coordinatedM6Move();
        }
        else if (longestMotor == "m6")
        {
            if (lastD2G != motor6->distanceToGo())
            {
                stepCounter += m6StepDelay;
                lastD2G = motor6->distanceToGo();
            }

            motor6->run();

            coordinatedM1Move();
            coordinatedM2Move();
            coordinatedM3Move();
            coordinatedM4Move();
            coordinatedM5Move();
        }

        if (motor1->distanceToGo() == 0 && motor2->distanceToGo() == 0 && motor3->distanceToGo() == 0 && motor4->distanceToGo() == 0 && motor5->distanceToGo() == 0 && motor6->distanceToGo() == 0)
        {
            // reset the stage of this function so that it can begin again when called again in the future
            stage = 0;
        }
    }
}

void MoveController::coordinatedM1Move()
{
    if (m1StepDelay > 0)
    {
        if ((m1StepCounter < stepCounter))
        {
            digitalWrite(STP1, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP1, LOW);

            motor1->setCurrentPosition(motor1->currentPosition() + motorM1Dir * 1);

            m1StepCounter += m1StepDelay;
        }
        else if (lastD2G == 0 && motor1->distanceToGo() > 0) // motor hasn't traveled forward enough yet
        {

            digitalWrite(DIR1, LOW); // set direction forward

            digitalWrite(STP1, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP1, LOW);

            motor1->setCurrentPosition(motor1->currentPosition() + 1);

            m1StepCounter += m1StepDelay;
        }
        else if (lastD2G == 0 && motor1->distanceToGo() < 0) // motor hasn't traveled backwards enough yet
        {

            digitalWrite(DIR1, HIGH); // set direction backwards

            digitalWrite(STP1, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP1, LOW);

            motor1->setCurrentPosition(motor1->currentPosition() - 1);

            m1StepCounter += m1StepDelay;
        }
    }
}

void MoveController::coordinatedM2Move() // if this is not the longest motor, it will perform a step every [stepDelay] number of steps compared to the longest motor
{
    if (m2StepDelay > 0)
    {
        if (m2StepCounter < stepCounter)
        {
            digitalWrite(STP2, HIGH);
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP2, LOW);

            motor2->setCurrentPosition(motor2->currentPosition() + motorM2Dir * 1);

            m2StepCounter += m2StepDelay;
        }
        else if (lastD2G == 0 && motor2->distanceToGo() > 0) // motor hasn't traveled right enough yet
        {

            digitalWrite(DIR2, HIGH); // clockwise
            digitalWrite(STP2, HIGH);
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP2, LOW);

            motor2->setCurrentPosition(motor2->currentPosition() + 1);

            m2StepCounter += m2StepDelay;
        }
        else if (lastD2G == 0 && motor2->distanceToGo() < 0) // motor hasn't traveled left enough yet
        {
            digitalWrite(DIR2, LOW); // counter clockwise
            digitalWrite(STP2, HIGH);
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP2, LOW);

            motor2->setCurrentPosition(motor2->currentPosition() - 1);

            m2StepCounter += m2StepDelay;
        }
    }
}

void MoveController::coordinatedM3Move()
{
    if (m3StepDelay > 0)
    {
        if ((m3StepCounter < stepCounter))
        {
            digitalWrite(STP3, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP3, LOW);

            motor3->setCurrentPosition(motor3->currentPosition() + motorM3Dir * 1);

            m3StepCounter += m3StepDelay;
        }
        else if (lastD2G == 0 && motor3->distanceToGo() > 0) // motor hasn't traveled forward enough yet
        {

            digitalWrite(DIR3, LOW); // set direction forward

            digitalWrite(STP3, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP3, LOW);

            motor3->setCurrentPosition(motor3->currentPosition() + 1);

            m3StepCounter += m3StepDelay;
        }
        else if (lastD2G == 0 && motor3->distanceToGo() < 0) // motor hasn't traveled backwards enough yet
        {

            digitalWrite(DIR3, HIGH); // set direction backwards

            // Serial.println("Motor Y Coordinated Pulse");
            digitalWrite(STP3, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP3, LOW);

            motor3->setCurrentPosition(motor3->currentPosition() - 1);

            m3StepCounter += m3StepDelay;
        }
    }
}

void MoveController::coordinatedM4Move()
{
    if (m4StepDelay > 0)
    {
        if ((m4StepCounter < stepCounter))
        {
            digitalWrite(STP4, HIGH);
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP4, LOW);

            motor4->setCurrentPosition(motor4->currentPosition() + motorM4Dir * 1);

            m4StepCounter += m4StepDelay;
        }
        else if (lastD2G == 0 && motor4->distanceToGo() > 0) // motor hasn't moved up enough
        {

            // Run the motor
            digitalWrite(DIR4, LOW); // counter clockwise = up
            digitalWrite(STP4, HIGH);
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP4, LOW);

            motor4->setCurrentPosition(motor4->currentPosition() + 1);

            m4StepCounter += m4StepDelay;
        }
        else if (lastD2G == 0 && motor4->distanceToGo() < 0) // motor hasn't down up enough
        {
            Serial.println("Z Hasn't Moved Down Enough. ");

            // Run the motor
            digitalWrite(DIR4, HIGH); // clockwise = down
            digitalWrite(STP4, HIGH);
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP4, LOW);

            motor4->setCurrentPosition(motor4->currentPosition() - 1);

            m4StepCounter += m4StepDelay;
        }
    }
}

void MoveController::coordinatedM5Move()
{
    if (m5StepDelay > 0)
    {
        if ((m5StepCounter < stepCounter))
        {
            digitalWrite(STP5, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP5, LOW);

            motor5->setCurrentPosition(motor5->currentPosition() + motorM5Dir * 1);

            m5StepCounter += m5StepDelay;
        }
        else if (lastD2G == 0 && motor5->distanceToGo() > 0) // motor hasn't traveled forward enough yet
        {

            digitalWrite(DIR5, LOW); // set direction forward

            digitalWrite(STP5, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP5, LOW);

            motor5->setCurrentPosition(motor5->currentPosition() + 1);

            m5StepCounter += m5StepDelay;
        }
        else if (lastD2G == 0 && motor5->distanceToGo() < 0) // motor hasn't traveled backwards enough yet
        {

            digitalWrite(DIR5, HIGH); // set direction backwards

            digitalWrite(STP5, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP5, LOW);

            motor5->setCurrentPosition(motor5->currentPosition() - 1);

            m5StepCounter += m5StepDelay;
        }
    }
}

void MoveController::coordinatedM6Move()
{
    if (m6StepDelay > 0)
    {
        if ((m6StepCounter < stepCounter))
        {
            digitalWrite(STP6, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP6, LOW);

            motor6->setCurrentPosition(motor6->currentPosition() + motorM6Dir * 1);

            m6StepCounter += m6StepDelay;
        }
        else if (lastD2G == 0 && motor6->distanceToGo() > 0) // motor hasn't traveled forward enough yet
        {

            digitalWrite(DIR6, LOW); // set direction forward

            digitalWrite(STP6, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP6, LOW);

            motor6->setCurrentPosition(motor6->currentPosition() + 1);

            m6StepCounter += m6StepDelay;
        }
        else if (lastD2G == 0 && motor6->distanceToGo() < 0) // motor hasn't traveled backwards enough yet
        {

            digitalWrite(DIR6, HIGH); // set direction backwards

            digitalWrite(STP6, HIGH); // run the motors with a minPulseWidth.
            delayMicroseconds(minPulseWidth);
            digitalWrite(STP6, LOW);

            motor6->setCurrentPosition(motor6->currentPosition() - 1);

            m6StepCounter += m6StepDelay;
        }
    }
}

bool MoveController::motorsAtTarget()
{
    return motor1->distanceToGo() == 0 &&
           motor2->distanceToGo() == 0 &&
           motor3->distanceToGo() == 0 &&
           motor4->distanceToGo() == 0 &&
           motor5->distanceToGo() == 0 &&
           motor6->distanceToGo() == 0;
}