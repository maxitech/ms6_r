#ifndef JOGCOMMAND_H
#define JOGCOMMAND_H

enum JogCommand
{
    JOG_START,  ///< Start jogging in the specified direction.
    JOG_STOP,   ///< Stop the current jog operation.
    JOG_UNKNOWN ///< Unknown jog command state.
};

#endif //  JOGCOMMAND_H