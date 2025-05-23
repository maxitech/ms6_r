#include "CommandProcessor.h"

CommandProcessor::CommandProcessor(ProgramLoader& programLoader)
    : _dispatcher(programLoader)
{
}

void CommandProcessor::processInput(const String& input)
{
    if (!_isInputValid(input))
    {
        Serial.println("Input is invalid. Expected input: $<cmd_pt1, cmd_pt2, etc.>*<checksum>#");
        delay(20);
        return;
    }

    int    delimiterIndex = input.indexOf("*");
    String data           = input.substring(1, delimiterIndex);
    String checksum       = input.substring(delimiterIndex + 1, input.length() - 1);

    if (!_validateChecksum(data, checksum))
    {
        Serial.println("Input processing failed: Checksum validation error.");
        delay(20);
        return;
    }

    _processCommand(data);
}

bool CommandProcessor::_isInputValid(const String& input)
{
    if (!input.startsWith("$") || !input.endsWith("#") || input.indexOf("*") == -1)
        return false;

    return true;
}

bool CommandProcessor::_validateChecksum(const String& data, const String& checksum)
{
    uint8_t checksumValue = 0;

    // Calculate checksum by XORing all characters in data -> (8-bit checksum)
    for (size_t i = 0; i < data.length(); i++)
    {
        checksumValue ^= static_cast<uint8_t>(data[i]);
    }

    String calculatedChecksum = String(checksumValue, HEX).toUpperCase();
    if (calculatedChecksum.length() == 1)
    {
        calculatedChecksum = "0" + calculatedChecksum;
    }

    if (calculatedChecksum != checksum)
    {
        Serial.println("Checksum Error. Expected: " + checksum + ", Calculated: " + calculatedChecksum);
        delay(20);
        return false;
    }

    return true;
}

void CommandProcessor::_processCommand(const String& cmd)
{
    std::pair<String, std::vector<String>> parts = _splitString(cmd);

    _dispatcher.dispatch(parts.first, parts.second);
}

std::pair<String, std::vector<String>> CommandProcessor::_splitString(const String& str)
{
    std::vector<String> tokens;
    int                 firstComma = str.indexOf(',');
    if (firstComma == -1)
    {
        return {str, tokens};
    }

    String command  = str.substring(0, firstComma);
    String paramStr = str.substring(firstComma + 1);

    if (paramStr.startsWith("[") && paramStr.endsWith("]"))
    {
        paramStr = paramStr.substring(1, paramStr.length() - 1); // Remove brackets
    }

    // Split parameters using comma
    int start = 0;
    int end   = paramStr.indexOf(',');
    while (end != -1)
    {
        tokens.push_back(paramStr.substring(start, end));
        start = end + 1;
        end   = paramStr.indexOf(',', start);
    }

    // Push last element
    tokens.push_back(paramStr.substring(start));

    return {command, tokens};
}