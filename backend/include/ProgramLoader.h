#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include <Arduino.h>


enum ProgramState {
    IDLE,
    PING,
    PONG
};


class ProgramLoader {
public: 
    ProgramLoader();
    void handleCommand(const std::vector<String>& cmdParts);
    void run();
    ProgramState getState() const;


private:
    void _loadProgram(const String& program);
    ProgramState _setState(ProgramState newState);
    void _executePing();
    void _executePong();

    ProgramState _currentProgramState = IDLE;
};


#endif //PROGRAMLOADER_H