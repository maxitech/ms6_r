#include "Setup.h"
using namespace TS4;

Setup& Setup::getInstance()
{
    static Setup instance; // Only instance of `Setup`
    return instance;
}

Setup::Setup()
    : // int stepPin, int dirPin
    motorJ1(motorJ1Step, motorJ1Dir)
    , motorJ2(motorJ2Step, motorJ2Dir)
    , motorJ3(motorJ3Step, motorJ3Dir)
    , motorJ4(motorJ4Step, motorJ4Dir)
    , motorJ5(motorJ5Step, motorJ5Dir)
    , motorJ6(motorJ6Step, motorJ6Dir)
    , limitSwitches(ledPin, limitSwitchPins)
    , homingManager(limitSwitchPins)
    , programLoader(&homingManager, motorConfigs, limitSwitches)
    , cmdProcessor(programLoader) {};

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

void Setup::update(const String& jsonString)
{
    if (jsonString.length() > 0)
    {
        _jsonStr = jsonString;
        _validateJson();
        std::vector<DHparam> extractedParams = _extractDHParams();

        if (extractedParams.size() == 6)
        {
            dhParams = extractedParams;
        }
        std::vector<int> extractedHomePositions = _extractHomingParams(); // Holding all home positions -> set via frontend setup
        if (extractedHomePositions.size() == 6)
        {
            HOME_POS_J1 = extractedHomePositions[0];
            HOME_POS_J2 = extractedHomePositions[1];
            HOME_POS_J3 = extractedHomePositions[2];
            HOME_POS_J4 = extractedHomePositions[3];
            HOME_POS_J5 = extractedHomePositions[4];
            HOME_POS_J6 = extractedHomePositions[5];
        }

        if (kin)
        {
            delete kin;
        }
        kin = new Kinematics(motorConfigs, dhParams);
    }
}

void Setup::init()
{
    pinMode(motorJ1En, OUTPUT);
    pinMode(motorJ2En, OUTPUT);
    pinMode(motorJ3En, OUTPUT);
    pinMode(motorJ4En, OUTPUT);
    pinMode(motorJ5En, OUTPUT);
    pinMode(motorJ6En, OUTPUT);

    // digitalWrite(motorJ1En, HIGH);
    // digitalWrite(motorJ2En, HIGH);
    // digitalWrite(motorJ3En, HIGH);
    // digitalWrite(motorJ4En, HIGH);
    // digitalWrite(motorJ5En, HIGH);
    // digitalWrite(motorJ6En, HIGH);

    TS4::begin();
    TimerFactory::attachModule(new TMRModule<0>());

    motorJ1
        .setMaxSpeed(50'000)
        .setAcceleration(26'000);

    motorJ2
        .setMaxSpeed(160'000)
        .setAcceleration(50'000);

    motorJ3
        .setMaxSpeed(60'000)
        .setAcceleration(30'000);

    motorJ4
        .setMaxSpeed(40'000)
        .setAcceleration(30'000);

    motorJ5
        .setMaxSpeed(40'000)
        .setAcceleration(20'000);

    motorJ6
        .setMaxSpeed(15'000)
        .setAcceleration(8000);

    // Create and add groups
    auto group1 = std::make_unique<AxisGroup>();
    group1->addAxis(&axis1);
    group1->addAxis(&axis2);
    group1->addAxis(&axis4);

    auto group2 = std::make_unique<AxisGroup>();
    group2->addAxis(&axis3);

    auto group3 = std::make_unique<AxisGroup>();
    group3->addAxis(&axis5);
    group3->addAxis(&axis6);

    homingManager.addGroup(std::move(group1));
    homingManager.addGroup(std::move(group2));
    homingManager.addGroup(std::move(group3));

    limitSwitches.init();
    serialHandler.setCommandProcessor(&cmdProcessor);
}