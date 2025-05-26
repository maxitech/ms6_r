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

//************* Old version of _readSerialinput() - commented out for referencej**************//
// void SerialHandler::_readSerialInput()
// {
//     const char DELIMITER = '#';
//     char       buffer[256];

//     size_t length = Serial.readBytesUntil(DELIMITER, buffer, sizeof(buffer) - 1);

//     if (length == sizeof(buffer) - 1)
//     {
//         Serial.println("Warning: input too long, might be truncated!");
//         return;
//     }
//     else if (length > 0)
//     {
//         buffer[length] = '\0';
//         String input   = String(buffer);
//         input.trim();
//         input += DELIMITER;
//         Serial.print("[DEBUG] Received command: '");
//         Serial.print(input);
//         _forwardInput(input);
//     }
// }

void SerialHandler::_readSerialInput()
{
    static char   buffer[256]; // static buffer to hold incoming data
    static size_t index = 0;   // current index in the buffer

    while (Serial.available() > 0)
    {
        char c = Serial.read();

        if (c == '#') // end delimeter
        {
            buffer[index] = '\0'; // Null-terminate the string

            String input = String(buffer);
            input.trim();
            input += '#';

            _forwardInput(input);

            index = 0; // reset buffer index
        }
        else
        {
            // Add char to buffer if there's space
            if (index < sizeof(buffer) - 1)
            {
                buffer[index++] = c;
            }
            else
            {
                // Buffer overflow, reset index
                index = 0;
                Serial.println("Error: Serial buffer overflow");
                delay(20);
                return;
            }
        }
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