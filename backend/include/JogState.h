/**
 * @file JogState.h
 * @brief This file contains a enum which holds `JogStates`.
 */

#ifndef JOGSTATE_H
#define JOGSTATE_H

enum JogState
{
    IDLE_JOG, ///< No jog command is active.
    JOGGING,  ///< Currently executing a jog command.
};

#endif // JOSTATE_H