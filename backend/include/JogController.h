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
#include <optional>

struct JogFlags
{
    bool   limitReached = false; ///< Flag to check if limit is reached.
    String blockedDir   = "";    ///< Sting of which direction is blocked -> "POS" == 1 || "NEG" == -1
    bool   runOnce      = false; ///< Flag to execute a function once.
};

class JogController
{
public:
    JogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender);

    /**
     * @brief Executes the jog command for a specific joint.
     * @param jogSpeeds Reference to the passed optional jogSpeeds vector.
     * @param currJogState Reference to the passed `JogState`.
     */
    bool jogJoint(std::optional<std::vector<int32_t>>& jogSpeeds, JogState& currJogState);

private:
    /**
     * @brief Parses the jog command from the input string.
     * @param str The jog command string.
     * @return The JogCommand enum value representing the jog command.
     * @internal
     */
    // JogCommand _getJogCommand(const int index);
    String _getDir(std::vector<int32_t>& jogSpeedsValid, int index);

    std::array<JogFlags, Utils::NUM_DOF> _jogFlags;     /// < Array holding `JogFlags` for each motor. @internal
    std::vector<MotorConfig*>&           _motorConfigs; ///< Reference to a Vector of motor config pointers for the robot. Note: Setup class has full ownership of this vector. @internal
    RobotDataSender&                     _rbtDtaSender; ///< Reference to the data sender class for the robot. @internal
    int                                  _activeIndex;
};

#endif // JOGCONTROLLER_h