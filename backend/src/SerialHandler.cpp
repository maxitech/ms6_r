#include <Arduino.h>
#include "SerialHandler.h"


SerialHandler::SerialHandler() : _command(""), _lastCommand("") {}


void SerialHandler::start(){
  _readSerialInput();

  // Check if the command should be executed repeatedly
  if (_shouldAlwaysExecuteCommand(_command)) {
    Serial.println("Executing cmd repeatedly: " + _command);
    _handleNewCommand();
    return;
  }

  // Check if the command is new and not in the whitelist: -> execute once
  if (_command != _lastCommand) {
    Serial.println("Executing cmd once: " + _command);
    _handleNewCommand();
    _lastCommand = _command;
  } else {
    Serial.println("Command already executed: " + _command);
  }
}


void SerialHandler::_readSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();
    Serial.println("Loaded Program - " + input);
    _command = input;
  }
}


/* Check if the command is in the list of commands
that should be executed repeatedly (e.g. "MOVE") */
bool SerialHandler::_shouldAlwaysExecuteCommand(const String& cmd) {
  return (cmd == "MOVE");
}


void SerialHandler::_handleNewCommand() {
  Serial.println("Processing command: " + _command);
}