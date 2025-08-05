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

//************* Old version of _readSerialinput() - commented out for reference **************//
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

// void SerialHandler::_readSerialInput()
// {
//     static char   buffer[256]; // static buffer to hold incoming data
//     static size_t index = 0;   // current index in the buffer

//     while (Serial.available() > 0)
//     {
//         char c = Serial.read();

//         if (c == '#') // end delimeter
//         {
//             buffer[index] = '\0'; // Null-terminate the string

//             String input = String(buffer);
//             input.trim();
//             input += '#';

//             _forwardInput(input);

//             index = 0; // reset buffer index
//         }
//         else
//         {
//             // Add char to buffer if there's space
//             if (index < sizeof(buffer) - 1)
//             {
//                 buffer[index++] = c;
//             }
//             else
//             {
//                 // Buffer overflow, reset index
//                 index = 0;
//                 Serial.println("Error: Serial buffer overflow");
//                 return;
//             }
//         }
//     }
// }

// void SerialHandler::_readSerialInput()
// {
//     static char   message[MAX_MESSAGE_SIZE];
//     static size_t index     = 0;
//     static bool   receiving = false;

//     while (Serial.available() > 0)
//     {
//         char c = Serial.read();

//         if (c == '$') // Start of message
//         {
//             receiving = true;
//             index     = 0; // Reset buffer
//         }

//         if (receiving)
//         {
//             if (index < MAX_MESSAGE_SIZE - 1)
//             {
//                 message[index++] = c;
//                 if (c == '#') // End of message
//                 {
//                     message[index] = '\0';
//                     receiving      = false;

//                     String input = String(message);
//                     input.trim();

//                     // _forwardInput(input); // Process input
//                     Serial.println(input);
//                     index = 0;
//                 }
//             }
//             else
//             {
//                 // Buffer overflow
//                 receiving = false;
//                 index     = 0;
//                 Serial.println("Error: Serial message too long");
//             }
//         }
//     }
// }

void SerialHandler::_readSerialInput()
{
    static char   message[MAX_MESSAGE_SIZE];
    static size_t index     = 0;
    static bool   receiving = false;

    while (Serial.available() > 0)
    {
        char c = Serial.read();

        if (c == '$') // Start of message
        {
            receiving = true;
            index     = 0; // Reset buffer
        }

        if (receiving)
        {
            if (index < MAX_MESSAGE_SIZE - 1)
            {
                message[index++] = c;
                if (c == '#') // End of message
                {
                    message[index] = '\0';
                    receiving      = false;

                    String input = String(message);
                    input.trim();

                    _forwardInput(input); // Process input
                    index = 0;
                }
            }
            else
            {
                // Buffer overflow
                receiving = false;
                index     = 0;
                Serial.println("Error: Serial message too long");
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
        Serial.println("Error: No CommandProcesor set. Please set one using setCommandProcessor() in Setup.cpp");
    }
}