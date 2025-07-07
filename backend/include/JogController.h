/**
 * @file JogController.h
 * @brief This controller class is used to handle jog related functionality.
 */

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
    bool   limitReached = false; ///< Flag to check if limit is reached.
    String blockedDir   = "";    ///< Sting of which direction is blocked -> "POS" || "NEG"
    bool   runOnce      = false; ///< Flag to execute a function once.
};

class JogController
{
public:
    JogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);

    /**
     * @brief Executes the jog command for a specific joint.
     * @param arguments Reference to the passed arguments vector.
     * @param currJogState Reference to the passed `JogState`.
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

    std::array<JogFlags, Utils::NUM_DOF> _jogFlags;     /// < Array holding `JogFlags` for each motor. @internal
    std::vector<MotorConfig*>&           _motorConfigs; ///< Reference to a Vector of motor config pointers for the robot. Note: Setup class has full ownership of this vector. @internal
    RobotDataSender&                     _rbtDtaSender; ///< Reference to the data sender class for the robot. @internal
};

#endif // JOGCONTROLLER_h