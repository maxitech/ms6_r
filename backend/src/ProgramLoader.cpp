#include "ProgramLoader.h"
#include <map>


ProgramLoader::ProgramLoader(LimitSwitches& limitSwitches) : _limitSwitches(limitSwitches) {};


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
    static const std::map<String, ProgramState> programMap = {
        {"PING", PING},
        {"PONG", PONG},
        {"TEST_SWITCHES", TEST_SWITCHES}
    };

    auto it = programMap.find(program);
    if(it == programMap.end()) {
        Serial.println("Unknown program: " + program);
        return;
    }

    if(_currentProgramState == it->second) {
        Serial.println("Reloaded program: " + program);
    } else {
        Serial.println("Loading program: " + program);
        _setState(it->second);
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
    case TEST_SWITCHES:
      _testSwitches();
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


void ProgramLoader::_testSwitches() {
    _limitSwitches.check();
}