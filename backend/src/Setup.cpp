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

std::vector<MotionProfile> Setup::_extractMotionProfiles()
{
    if (!_valid)
    {
        Serial.println("Error: JSON invalid, cannot parse motionProfile params.");
        return {};
    }

    JsonObjectConst            speed_a_accel = _jsonDoc["speed_a_accel"];
    std::vector<MotionProfile> result;

    for (int i = 1; i <= 6; ++i)
    {
        std::string     motor         = "motor" + std::to_string(i);
        JsonObjectConst motionProfile = speed_a_accel[motor.c_str()];

        if (!_checkExists(motionProfile, motor.c_str()))
        {
            return {};
        }

        const char* max_speed = motionProfile["max_speed"];
        const char* acc       = motionProfile["acc"];

        if (!max_speed || !acc)
        {
            Serial.println(("Error: missing field in " + String(motor.c_str())));
            return {};
        }

        MotionProfile profile;
        profile.max_speed = std::stoi(max_speed);
        profile.accel     = std::stoi(acc);

        result.push_back(profile);
    }
    return result;
}

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
        std::vector<MotionProfile> extractedProfiles = _extractMotionProfiles();
        if (extractedProfiles.size() == 6)
        {
            Stepper* motors[] = {&motorJ1, &motorJ2, &motorJ3, &motorJ4, &motorJ5, &motorJ6};
            for (size_t i = 0; i < extractedProfiles.size(); ++i)
            {
                motors[i]->setMaxSpeed(extractedProfiles[i].max_speed).setAcceleration(extractedProfiles[i].accel);
            }
        }

        for (auto cfg : motorConfigs)
        {
            delete cfg;
        }
        motorConfigs.clear();
        motorConfigs.reserve(6);
        motorConfigs.push_back(new MotorConfig {&motorJ1, HOME_POS_J1, 200, 64, 16.0f, 100.0f, 1.0f});
        motorConfigs.push_back(new MotorConfig {&motorJ2, HOME_POS_J2, 200, 64, 16.0f, 80.0f, 14.0f});
        motorConfigs.push_back(new MotorConfig {&motorJ3, HOME_POS_J3, 200, 64, 16.0f, 100.0f, 1.0f});
        motorConfigs.push_back(new MotorConfig {&motorJ4, HOME_POS_J4, 200, 64, 16.0f, 60.0f, 1.0f});
        motorConfigs.push_back(new MotorConfig {&motorJ5, HOME_POS_J5, 400, 64, 16.0f, 32.0f, 1.0f});
        motorConfigs.push_back(new MotorConfig {&motorJ6, HOME_POS_J6, 200, 64, 1.0f, 1.0f, 1.0f});

        if (kin)
            delete kin;

        kin = new Kinematics(motorConfigs, dhParams);

        if (axis1)
            delete axis1;
        if (axis2)
            delete axis2;
        if (axis3)
            delete axis3;
        if (axis4)
            delete axis4;
        if (axis5)
            delete axis5;
        if (axis6)
            delete axis6;

        axis1 = new AxisData {MOVE_TO_SWITCH, &motorJ1, J1, HOMING_VELOCITY_J1, MOVE_AWAY_VELOCITY_J1, MOVE_BACK_VELOCITY_J1, HOME_POS_J1};
        axis2 = new AxisData {MOVE_TO_SWITCH, &motorJ2, J2, HOMING_VELOCITY_J2, MOVE_AWAY_VELOCITY_J2, MOVE_BACK_VELOCITY_J2, HOME_POS_J2};
        axis3 = new AxisData {MOVE_TO_SWITCH, &motorJ3, J3, HOMING_VELOCITY_J3, MOVE_AWAY_VELOCITY_J3, MOVE_BACK_VELOCITY_J3, HOME_POS_J3};
        axis4 = new AxisData {MOVE_TO_SWITCH, &motorJ4, J4, HOMING_VELOCITY_J4, MOVE_AWAY_VELOCITY_J4, MOVE_BACK_VELOCITY_J4, HOME_POS_J4};
        axis5 = new AxisData {MOVE_TO_SWITCH, &motorJ5, J5, HOMING_VELOCITY_J5, MOVE_AWAY_VELOCITY_J5, MOVE_BACK_VELOCITY_J5, HOME_POS_J5};
        axis6 = new AxisData {MOVE_TO_SWITCH, &motorJ6, J6, HOMING_VELOCITY_J6, MOVE_AWAY_VELOCITY_J6, MOVE_BACK_VELOCITY_J6, HOME_POS_J6};

        if (!axis1 || !axis2 || !axis3 || !axis4 || !axis5 || !axis6)
        {
            Serial.println("Error: Failed to create AxisData objects!");
            return;
        }

        // Clear existing groups
        homingManager.clearGroups();

        // Create and add groups
        auto group1 = std::make_unique<AxisGroup>();
        group1->addAxis(axis1);
        group1->addAxis(axis2);
        group1->addAxis(axis4);

        auto group2 = std::make_unique<AxisGroup>();
        group2->addAxis(axis3);

        auto group3 = std::make_unique<AxisGroup>();
        group3->addAxis(axis5);
        group3->addAxis(axis6);

        homingManager.addGroup(std::move(group1));
        homingManager.addGroup(std::move(group2));
        homingManager.addGroup(std::move(group3));
    }
}

void Setup::init()
{
    // Runs in setup() of main.cpp
    TS4::begin();
    TimerFactory::attachModule(new TMRModule<0>());

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

    limitSwitches.init();
    serialHandler.setCommandProcessor(&cmdProcessor);
}