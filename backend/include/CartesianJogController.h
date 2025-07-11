#ifndef CARTESIANJOGCONTROLLER_H
#define CARTESIANJOGCONTROLLER_H

#include "JogState.h"
#include "Kinematics.h"
#include "MotorConfig.h"
#include "RobotDataSender.h"

class CartesianJogController
{
public:
    CartesianJogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender, Kinematics* kin);
    // need motorConfigs, arguments, motorIdx & rbtDataSender(sendPosData) from program loader maybe getJogCmd from JogController
    void jog_cartesian(const std::vector<String>& arguments, JogState& currJogState);

private:
    void   _handle_jog_cartesian(const std::vector<String>& arguments, JogState& currJogState);
    Pose   _getCurrPose();
    float  _calcIntervall(const float delta, const float speed);
    Pose   _calcNextPos(const String& mode, const String& axis, const float delta, const Pose& currPos);
    Angles _calcNextPosAngles(const Pose& nextPose);

    std::vector<MotorConfig*>& _motorConfigs; ///< Reference to a Vector of motor config pointers for the robot. Note: Setup class has full ownership of this vector. @internal
    RobotDataSender&           _rbtDtaSender; ///< Reference to the data sender class for the robot. @internal
    Kinematics*                _kin;          ///< Pointer to kinematics class instance. Note: Setup class has ownership.
};

#endif // CARTESIANJOGCONTROLLER_H