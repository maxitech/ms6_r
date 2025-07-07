#ifndef JOGCONTROLLER_H
#define JOGCONTROLLER_H

#include "JogCommand.h"
#include "JogState.h"
#include "MotorConfig.h"
#include "RobotDataSender.h"
#include "Utils.h"
#include <Arduino.h>

struct JogFlags
{
    bool   limitReached = false;
    String blockedDir   = "";
    bool   runOnce      = false;
};

class JogController
{
public:
    JogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);

    /**
     * @brief Executes the jog command for a specific joint.
     * @param currJogState Reference to the current JogState.
     * @param motorIdx The index of the motor to jog.
     */
    void jogJoint(std::vector<String>& arguments, JogState& currJogState, const int motorIdx);

private:
    /**
     * @brief Parses the jog command from the input string.
     * @param str The jog command string.
     * @return The JogCommand enum value representing the jog command.
     * @internal
     */
    JogCommand _getJogCommand(const String& str);

    std::array<JogFlags, Utils::NUM_DOF> _jogFlags;
    std::vector<MotorConfig*>&           _motorConfigs;
    RobotDataSender&                     _rbtDtaSender;
};

#endif // JOGCONTROLLER_h