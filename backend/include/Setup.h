#ifndef SETUP_H
#define SETUP_H

#include "Kinematics.h"
#include <ArduinoJson.h>

class Setup
{
public:
    explicit Setup(const String& jsonString);

private:
    std::vector<DHparam> _parseDHParams();
    std::vector<int>     _parseHomingParams();

    const String _jsonStr;
};

#endif // SETUP_H
