#include "SerialHandler.h"
#include "CommandProcessor.h"

SerialHandler::SerialHandler() {}

void SerialHandler::setCommandProcessor(CommandProcessor* processor)
{
    _commandProcessor = processor;
}

void SerialHandler::listenForSerial()
{
    _readSerialInput();
}

void SerialHandler::_readSerialInput()
{
    if (Serial.available() > 0)
    {
        String input = Serial.readString();
        input.trim();
        if (input.length() == 0)
            return;
        _forwardInput(input);
    }
}

void SerialHandler::_forwardInput(const String& input)
{
    if (_commandProcessor)
    {
        _commandProcessor->processInput(input);
    }
    else
    {
        Serial.println("Error: No CommandProcesor set. Please set one using setCommandProcessor() in main.cpp");
        delay(20);
    }
}