#ifndef CARTESIANJOGCONTROLLER_H
#define CARTESIANJOGCONTROLLER_H

#include "JogCommand.h"
#include "JogState.h"
#include "Kinematics.h"
#include "MotorConfig.h"
#include "RobotDataSender.h"

#include "JogController.h"

class CartesianJogController
{
public:
    CartesianJogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);
    // need motorConfigs, arguments, motorIdx & rbtDataSender(sendPosData) from program loader maybe getJogCmd from JogController
    void jog_cartesian(const std::vector<String>& arguments, JogState& currJogState);

    void setKin(Kinematics* kin)
    {
        _kin = kin;
    }

private:
    void             _handle_jog_cartesian(const std::vector<String>& arguments, JogState& currJogState);
    Pose             _getCurrPose();
    float            _calcIntervall(const float delta, const float speed);
    Pose             _calcNextPos(const String& mode, const String& axis, const float delta, const int dir, const Pose& currPos);
    Angles           _calcNextPosAngles(const Pose& nextPose);
    void             _moveToPos(const std::vector<String>& arguments);
    std::vector<int> _convertAngToSteps(const Angles& angles);
    JogCommand       _getJogCommand(const String& str);

    std::vector<MotorConfig*>&           _motorConfigs; ///< Reference to a Vector of motor config pointers for the robot. Note: Setup class has full ownership of this vector. @internal
    RobotDataSender&                     _rbtDtaSender; ///< Reference to the data sender class for the robot. @internal
    Kinematics*                          _kin;          ///< Pointer to kinematics class instance. Note: Setup class has ownership.
    JogState                             _currJogState;
    std::array<JogFlags, Utils::NUM_DOF> _jogFlags;          /// < Array holding `JogFlags` for each motor. @internal
    Pose                                 _lockedOrientation; ///< Saving only roll, pitch, yaw

    //!!!!!!
    bool _isJogging = false;
    //!!!
};

#endif // CARTESIANJOGCONTROLLER_H