#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H


#include <Arduino.h>
#include "CommandProcessor.h"


class SerialHandler {
public:
    SerialHandler();
    void listenForSerial();
    void setCommandProcessor(CommandProcessor* processor);
    
    
private:
    void _readSerialInput();
    void _forwardInput(const String& input);


    CommandProcessor* _commandProcessor = nullptr;
};


#endif // SERIAL_HANDLER_H
