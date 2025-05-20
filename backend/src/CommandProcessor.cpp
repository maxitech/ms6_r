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
        return;
    }

    int    delimiterIndex = input.indexOf("*");
    String data           = input.substring(1, delimiterIndex);
    String checksum       = input.substring(delimiterIndex + 1, input.length() - 1);

    if (!_validateChecksum(data, checksum))
    {
        Serial.println("Input processing failed: Checksum validation error.");
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
        return false;
    }

    return true;
}

void CommandProcessor::_processCommand(const String& cmd)
{
    std::vector<String> parts = _splitString(cmd, ',');

    _dispatcher.dispatch(parts);
}

std::vector<String> CommandProcessor::_splitString(const String& str, const char delimiter)
{
    std::vector<String> tokens;
    int                 start = 0;
    int                 end   = str.indexOf(delimiter);

    while (end != -1)
    {
        tokens.push_back(str.substring(start, end));
        start = end + 1;
        end   = str.indexOf(delimiter, start);
    }

    tokens.push_back(str.substring(start));
    return tokens;
}