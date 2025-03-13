#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H


#include <Arduino.h>


#include "SerialHandler.h"
#include "LimitSwitches.h"


class CommandProcessor {
public:
    CommandProcessor();
    void processInput(const String& input);


private:
    bool _isInputValid(const String& input);
    void _processCommand(const String& cmd);
    bool _validateChecksum(const String& data, const String& checksum);
    std::vector<String> _splitString(const String& str, const char delimiter);
};


#endif // COMMANDPROCESSOR_H
