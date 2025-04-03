#ifndef HOMING_H
#define HOMING_H

#include <Arduino.h>

#include <memory>

#include "AxisGroup.h"

class Homing
{
public:
    explicit Homing(const std::array<byte, 6> &limitSwitches);
    void addGroup(std::unique_ptr<AxisGroup> group);
    void executeHoming();
    void resetGroup(AxisGroup &group);
    void resetAllGroups();

private:
    uint8_t _getActiveSwitches();
    bool _debounceRead(byte pin);
    void _updateSwitchStatus(Axes axis, bool active);
    void _homeAxis(bool isCurrentlyActive, bool wasPreviouslyActive, AxisData &axisData);

    const std::array<byte, 6> _limitSwitches;
    std::vector<std::unique_ptr<AxisGroup>> _axisGroups;
    uint8_t _PREVIOUS_SWITCH_STATUS = 0; // Holds the previous state of the switches
    uint8_t _SWITCH_STATUS = 0;          // Status for all 6 switches default: 0=(all inactive)
};

#endif // HOMING_H