#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "LimitSwitches.h"
#include <Arduino.h>

enum ProgramState
{
    IDLE,
    PING,
    PONG,
    TEST_SWITCHES
};

class ProgramLoader
{
public:
    explicit ProgramLoader(LimitSwitches& limitSwitches);
    void         handleCommand(const std::vector<String>& cmdParts);
    void         run();
    ProgramState getState() const;

private:
    void         _loadProgram(const String& program);
    ProgramState _setState(ProgramState newState);
    void         _executePing();
    void         _executePong();
    void         _testSwitches();

    ProgramState   _currentProgramState = IDLE;
    LimitSwitches& _limitSwitches;
};

#endif // PROGRAMLOADER_H