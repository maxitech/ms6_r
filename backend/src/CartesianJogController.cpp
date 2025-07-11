#include "CartesianJogController.h"

CartesianJogController::CartesianJogController(std::vector<MotorConfig*>& motorConfigs, RobotDataSender& rbtDtaSender)
    : _motorConfigs(motorConfigs)
    , _rbtDtaSender(rbtDtaSender) {};

void CartesianJogController::jog_cartesian(const std::vector<String>& arguments, JogState& currJogState)
{
    _handle_jog_cartesian(arguments, currJogState);
}

void CartesianJogController::_handle_jog_cartesian(const std::vector<String>& arguments, JogState& currJogState)
{
    // _arguments -> AXIS,MODE,DIR(-1/1),SPEED,DELTA,EVENT -> {X,LIN,1,15.5,5,START}
    const String& axis  = arguments[0];
    const String& mode  = arguments[1];
    const int     dir   = arguments[2].toInt();
    const float   speed = arguments[3].toFloat();
    const float   delta = arguments[4].toFloat();
    const String& event = arguments.back();
}