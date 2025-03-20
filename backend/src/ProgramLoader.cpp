#include "ProgramLoader.h"


ProgramLoader::ProgramLoader() {};


void ProgramLoader::handleCommand(const std::vector<String>& cmdParts) {
    if (cmdParts.size() < 2) {
        Serial.println("Error: Not enough arguments.");
        return;
    }

    const String command = cmdParts[0];
    const String program = cmdParts[1];

    if(command == "LOAD") {
        _loadProgram(program);
    }
}


void ProgramLoader::_loadProgram(const String& program) {
    if (program == "PING") {
        if (_currentProgramState == PING) {
            Serial.println("Reloading...");
        } else {
            Serial.println("Loading PING program...");
        }
        _setState(PING);
    } else if (program == "PONG") {
        if (_currentProgramState == PONG) {
            Serial.println("Reloading...");
        } else {
            Serial.println("Loading PONG program...");
        }
        _setState(PONG);
    } else {
        Serial.println("Unknown program: " + program);
    }
}


void ProgramLoader::run() {
  switch (_currentProgramState) {
    case PING:
      _executePing();
      break;
    case PONG:
      _executePong();
      break;
    case IDLE:
    default:
      break;
  }
}


ProgramState ProgramLoader::_setState(ProgramState newState) {
    _currentProgramState = newState;
    return _currentProgramState;
}


ProgramState ProgramLoader::getState() const {
  return _currentProgramState;
}


void ProgramLoader::_executePing() {
    static unsigned long lastPingTime = 0;
    if (millis() - lastPingTime > 1000) {
        lastPingTime = millis();
        Serial.println("PONG");
    }
}


void ProgramLoader::_executePong() {
    static unsigned long lastPongTime = 0;
    if (millis() - lastPongTime > 1000) {
        lastPongTime = millis();
        Serial.println("PING");
    }
}