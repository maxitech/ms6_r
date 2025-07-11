#ifndef CARTESIANJOGCONTROLLER_H
#define CARTESIANJOGCONTROLLER_H

#include "JogState.h"
#include "MotorConfig.h"
#include "RobotDataSender.h"

class CartesianJogController
{
public:
    CartesianJogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);
    // need motorConfigs, arguments, motorIdx & rbtDataSender(sendPosData) from program loader maybe getJogCmd from JogController
    void jog_cartesian(const std::vector<String>& arguments, JogState& currJogState);

    // currPos -> FK()
    // take in dir -> input from frontend dir==pressedBtn
    // new_pos = delta => delta + curr pos val -> which axis -> pressed btn
    // put new_pos in IK() -> get next joint angles
    // move all joints simultaniously to next pos
    // pay attention to joint limits and futre workspace limits

private:
    void  _handle_jog_cartesian(const std::vector<String>& arguments, JogState& currJogState);
    Pose  _getCurrPose();
    float _calcIntervall(const float delta, const float speed);
    Pose  _calcNextPos(const String& mode, const String& axis, const float delta, const Pose& currPos);

    std::vector<MotorConfig*>& _motorConfigs; ///< Reference to a Vector of motor config pointers for the robot. Note: Setup class has full ownership of this vector. @internal
    RobotDataSender&           _rbtDtaSender; ///< Reference to the data sender class for the robot. @internal
    Kinematics*                _kin;          ///< Pointer to kinematics class instance. Note: Setup class has ownership.
};

#endif // CARTESIANJOGCONTROLLER_H