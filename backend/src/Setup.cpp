#include "Setup.h"
using namespace TS4;

Setup::Setup(const String& jsonString)
    : _jsonStr(jsonString)
{
    _validateJson();
    _extractDHParams();
    _extractHomingParams();
};

void Setup::_validateJson()
{
    DeserializationError error = deserializeJson(_jsonDoc, _jsonStr);
    if (error)
    {
        Serial.println("Error: JSON Deserialization failed!");
        _valid = false;
    }
    else
    {
        _valid = true;
    }
}

std::vector<DHparam> Setup::_extractDHParams()
{
    if (!_valid)
    {
        Serial.println("Error: JSON invalid, cannot parse DH params.");
        return {};
    }

    JsonObjectConst      dh_params = _jsonDoc["dh_params"];
    std::vector<DHparam> result;

    for (int i = 1; i <= 6; ++i)
    {
        std::string     jointName = "joint" + std::to_string(i);
        JsonObjectConst joint     = dh_params[jointName.c_str()];

        if (!_checkExists(joint, jointName.c_str()))
        {
            return {};
        }

        const char* theta_offset_str = joint["theta_offset"];
        const char* alpha_str        = joint["alpha"];
        const char* d_str            = joint["d"];
        const char* a_str            = joint["a"];

        if (!_checkFields(theta_offset_str, alpha_str, d_str, a_str, jointName.c_str()))
        {
            return {};
        }

        DHparam param;
        param.thetaOffset = std::strtod(theta_offset_str, nullptr);
        param.alpha       = std::strtod(alpha_str, nullptr);
        param.d           = std::strtod(d_str, nullptr);
        param.a           = std::strtod(a_str, nullptr);

        result.push_back(param);
    }
    return result;
};

std::vector<int> Setup::_extractHomingParams()
{
    if (!_valid)
    {
        Serial.println("Error: JSON invalid, cannot parse homing params.");
        return {};
    }

    JsonObjectConst  homing_params = _jsonDoc["homing_params"];
    std::vector<int> result;

    for (int i = 1; i <= 6; ++i)
    {
        std::string     motor     = "motor" + std::to_string(i);
        JsonObjectConst motorData = homing_params[motor.c_str()];

        if (!_checkExists(motorData, motor.c_str()))
        {
            return {};
        }

        const char* home_pos = motorData["home_pos"];

        if (!home_pos)
        {
            Serial.println(("Error: missing field in " + String(motor.c_str())));
            return {};
        }

        result.push_back(std::stoi(home_pos));
    }
    return result;
};

bool Setup::_checkExists(const JsonObjectConst& obj, const char* name)
{
    if (obj.isNull())
    {
        Serial.println(("Error: missing object: " + String(name)));
        return false;
    }
    return true;
}

bool Setup::_checkFields(const char* f1, const char* f2, const char* f3, const char* f4, const char* context)
{
    if (!f1 || !f2 || !f3 || !f4)
    {
        Serial.println(("Error: missing field in " + String(context)));
        return false;
    }
    return true;
}