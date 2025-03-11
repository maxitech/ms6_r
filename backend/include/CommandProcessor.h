#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H


#include <Arduino.h>


#include "SerialHandler.h"
#include "LimitSwitches.h"


class CommandProcessor {
public:
    CommandProcessor(SerialHandler& serialHandler, LimitSwitches& limitSwitches);
    void processCommand(const String& command);


private:
    SerialHandler& serialHandler;
    LimitSwitches& limitSwitches;
    
    
    void processPingCmd();
    void processTestSwitchesCmd();

};


#endif // COMMANDPROCESSOR_H
