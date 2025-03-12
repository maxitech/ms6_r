#include "CommandProcessor.h"
#include "SerialHandler.h"


CommandProcessor::CommandProcessor(){}


void CommandProcessor::processInput(const String& input) {
  if(!_isInputValid(input)) {
    Serial.println("Input is invalid. Expected input: $<cmd>*<checksum>#");
    return;
  }

  int delimiterIndex = input.indexOf("*");
  String data = input.substring(1, delimiterIndex);
  String checksum = input.substring(delimiterIndex + 1, input.length() - 1);

  if (!_validateChecksum(data, checksum)) {
    Serial.println("Input processing failed: Checksum validation error.");
    return;
  }

  _processCommand(input);
}


bool CommandProcessor::_isInputValid(const String& input) {
  if(!input.startsWith("$") || !input.endsWith("#") || input.indexOf("*") == -1) {
    Serial.println("Command is invalid");
    return false;
  }

  return true;
}


bool CommandProcessor::_validateChecksum(const String& data, const String& checksum) {
  uint8_t checksumValue = 0;
  for(size_t i = 0; i < data.length(); i++) {
    checksumValue ^= static_cast<uint8_t>(data[i]);
  }

  String calculatedChecksum = String(checksumValue, HEX);

  if(calculatedChecksum.toUpperCase() != checksum) {
    Serial.println("Checksum Error. Expected: " + checksum + ", Calculated: " + calculatedChecksum.toUpperCase());
    return false;
  }

  return true;
}


void CommandProcessor::_processCommand(const String& cmd) {
  // process command
}
