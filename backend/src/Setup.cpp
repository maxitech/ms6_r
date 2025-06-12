#include "Setup.h"

Setup::Setup(const String& jsonString)
    : _jsonStr(jsonString)
{
    Setup::_parseDHParams();
};

std::vector<DHparam> Setup::_parseDHParams()
{
    JsonDocument         jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, _jsonStr);
    if (error)
    {
        Serial.println("Error: JSON Deserialization failed!");
        return {};
    }

    JsonObject           dh_params = jsonDoc["dh_params"];
    std::vector<DHparam> result;

    for (int i = 1; i <= 6; ++i)
    {
        std::string jointName = "joint" + std::to_string(i);
        JsonObject  joint     = dh_params[jointName.c_str()];

        if (joint.isNull())
        {
            Serial.println(("Error: missing joint: " + jointName).c_str());
            return {};
        }

        // Validate: all fields must exist and contain numeric values
        const char* theta_offset_str = joint["theta_offset"];
        const char* alpha_str        = joint["alpha"];
        const char* d_str            = joint["d"];
        const char* a_str            = joint["a"];

        if (!theta_offset_str || !alpha_str || !d_str || !a_str)
        {
            Serial.println(("Error: missing field in " + jointName).c_str());
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