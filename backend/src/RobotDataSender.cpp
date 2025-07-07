#include "RobotDataSender.h"
#include "Setup.h"

RobotDataSender::RobotDataSender() {};

void RobotDataSender::sendFkPoseAndJointAngles()
{
    Pose                pose        = Setup::getInstance().getKinematics()->forwardKinematics();
    std::vector<double> jointAngles = Setup::getInstance().getKinematics()->getJointAnglesInRadOrDeg(0);

    // Send forward kinematics pose
    // Format: DATA:FK_POSE*x,y,z,roll,pitch,yaw
    Serial.print("DATA:FK_POSE*");
    Serial.print(pose.x);
    Serial.print(",");
    Serial.print(pose.y);
    Serial.print(",");
    Serial.print(pose.z);
    Serial.print(",");
    Serial.print(pose.roll);
    Serial.print(",");
    Serial.print(pose.pitch);
    Serial.print(",");
    Serial.println(pose.yaw);

    if (jointAngles.empty())
    {
        Serial.println("Error: jointAngles vector is empty!");
        return;
    }
    // Send joint angles
    // Format: DATA:JOINT_ANGLES*angle1,angle2,angle3
    Serial.print("DATA:JOINT_ANGLES*");
    for (size_t i = 0; i < jointAngles.size(); ++i)
    {
        Serial.print(jointAngles[i]);
        if (i < jointAngles.size() - 1)
        {
            Serial.print(",");
        }
    }
    Serial.println();
}

void RobotDataSender::sendMotorPosInSteps(const MotorConfig* motorCoinfig, const int motorIdx)
{
    // Format: DATA:MOTOR_POS_STEPS*<motor_index>#<position>
    const int motorPos = motorCoinfig->motor->getPosition();
    Serial.print("DATA:MOTOR_POS_STEPS*");
    Serial.print(motorIdx + 1);
    Serial.print(",");
    Serial.println(motorPos);
}
