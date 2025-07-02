#include "Setup.h"
using namespace TS4;

Setup& Setup::getInstance()
{
    static Setup instance; // Only instance of `Setup`
    return instance;
}

Setup::Setup()
    : _motorJ1(_motorJ1Step, _motorJ1Dir) // int stepPin, int dirPin
    , _motorJ2(_motorJ2Step, _motorJ2Dir)
    , _motorJ3(_motorJ3Step, _motorJ3Dir)
    , _motorJ4(_motorJ4Step, _motorJ4Dir)
    , _motorJ5(_motorJ5Step, _motorJ5Dir)
    , _motorJ6(_motorJ6Step, _motorJ6Dir)
    , _limitSwitches(_ledPin, _limitSwitchPins)
    , _homingManager(_limitSwitchPins)
    , _programLoader(&_homingManager, _motorConfigs, _limitSwitches)
    , _cmdProcessor(_programLoader) {};

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

std::array<DHparam, 6> Setup::_extractDHParams()
{
    if (!_valid)
    {
        Serial.println("Error: JSON invalid, cannot parse DH params.");
        return {};
    }

    JsonObjectConst        dh_params = _jsonDoc["dh_params"];
    std::array<DHparam, 6> result;

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

        result[i - 1] = param; // Store in array, index 0 corresponds to joint1
    }
    return result;
};

std::array<int, 6> Setup::_extractHomingParams()
{
    if (!_valid)
    {
        Serial.println("Error: JSON invalid, cannot parse homing params.");
        return {};
    }

    JsonObjectConst    homing_params = _jsonDoc["homing_params"];
    std::array<int, 6> result;

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

        result[i - 1] = std::stoi(home_pos); // Store in array, index 0 corresponds to motor1
    }
    return result;
};

std::array<MotionProfile, 6> Setup::_extractMotionProfiles()
{
    if (!_valid)
    {
        Serial.println("Error: JSON invalid, cannot parse motionProfile params.");
        return {};
    }

    JsonObjectConst              speed_a_accel = _jsonDoc["speed_a_accel"];
    std::array<MotionProfile, 6> result;

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

        result[i - 1] = profile; // Store in array, index 0 corresponds to motor1
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

void Setup::_updateDhParams()
{
    std::array<DHparam, 6> extractedParams = _extractDHParams();

    if (extractedParams.size() == 6)
    {
        _dhParams = extractedParams;
    }
}

void Setup::_updateHomePositions()
{
    std::array<int, 6> extractedHomePositions = _extractHomingParams(); // Holding all home positions -> set via frontend setup
    if (extractedHomePositions.size() == 6)
    {
        _HOME_POS_J1 = extractedHomePositions[0];
        _HOME_POS_J2 = extractedHomePositions[1];
        _HOME_POS_J3 = extractedHomePositions[2];
        _HOME_POS_J4 = extractedHomePositions[3];
        _HOME_POS_J5 = extractedHomePositions[4];
        _HOME_POS_J6 = extractedHomePositions[5];
    }
}

void Setup::_updateMotionProfiles()
{
    std::array<MotionProfile, 6> extractedProfiles = _extractMotionProfiles();
    if (extractedProfiles.size() == 6)
    {
        Stepper* motors[] = {&_motorJ1, &_motorJ2, &_motorJ3, &_motorJ4, &_motorJ5, &_motorJ6};
        for (size_t i = 0; i < extractedProfiles.size(); ++i)
        {
            motors[i]->setMaxSpeed(extractedProfiles[i].max_speed).setAcceleration(extractedProfiles[i].accel);
        }
    }
}

void Setup::_updateMotorConfigs()
{
    for (auto cfg : _motorConfigs)
    {
        delete cfg;
    }
    _motorConfigs.clear();
    _motorConfigs.reserve(6);
    _motorConfigs.push_back(new MotorConfig {&_motorJ1, _HOME_POS_J1, 200, 64, 16.0f, 100.0f, 1.0f});
    _motorConfigs.push_back(new MotorConfig {&_motorJ2, _HOME_POS_J2, 200, 64, 16.0f, 80.0f, 14.0f});
    _motorConfigs.push_back(new MotorConfig {&_motorJ3, _HOME_POS_J3, 200, 64, 16.0f, 100.0f, 1.0f});
    _motorConfigs.push_back(new MotorConfig {&_motorJ4, _HOME_POS_J4, 200, 64, 16.0f, 60.0f, 1.0f});
    _motorConfigs.push_back(new MotorConfig {&_motorJ5, _HOME_POS_J5, 400, 64, 16.0f, 32.0f, 1.0f});
    _motorConfigs.push_back(new MotorConfig {&_motorJ6, _HOME_POS_J6, 200, 64, 1.0f, 1.0f, 1.0f});
}

void Setup::_updateKinematics()
{
    Kinematics* newKin = new Kinematics(_motorConfigs, _dhParams);
    if (!newKin)
    {
        Serial.println("Error: Failed to allocate Kinematics");
        return;
    }
    delete _kin;
    _kin = newKin;
}

void Setup::_updateAxisData()
{
    AxisData* newAxis1 = new AxisData {MOVE_TO_SWITCH, &_motorJ1, J1, _HOMING_VELOCITY_J1, _MOVE_AWAY_VELOCITY_J1, _MOVE_BACK_VELOCITY_J1, _HOME_POS_J1};
    AxisData* newAxis2 = new AxisData {MOVE_TO_SWITCH, &_motorJ2, J2, _HOMING_VELOCITY_J2, _MOVE_AWAY_VELOCITY_J2, _MOVE_BACK_VELOCITY_J2, _HOME_POS_J2};
    AxisData* newAxis3 = new AxisData {MOVE_TO_SWITCH, &_motorJ3, J3, _HOMING_VELOCITY_J3, _MOVE_AWAY_VELOCITY_J3, _MOVE_BACK_VELOCITY_J3, _HOME_POS_J3};
    AxisData* newAxis4 = new AxisData {MOVE_TO_SWITCH, &_motorJ4, J4, _HOMING_VELOCITY_J4, _MOVE_AWAY_VELOCITY_J4, _MOVE_BACK_VELOCITY_J4, _HOME_POS_J4};
    AxisData* newAxis5 = new AxisData {MOVE_TO_SWITCH, &_motorJ5, J5, _HOMING_VELOCITY_J5, _MOVE_AWAY_VELOCITY_J5, _MOVE_BACK_VELOCITY_J5, _HOME_POS_J5};
    AxisData* newAxis6 = new AxisData {MOVE_TO_SWITCH, &_motorJ6, J6, _HOMING_VELOCITY_J6, _MOVE_AWAY_VELOCITY_J6, _MOVE_BACK_VELOCITY_J6, _HOME_POS_J6};

    if (!newAxis1 || !newAxis2 || !newAxis3 || !newAxis4 || !newAxis5 || !newAxis6)
    {
        Serial.println("Error: Failed to allocate AxisData");
        delete newAxis1;
        delete newAxis2;
        delete newAxis3;
        delete newAxis4;
        delete newAxis5;
        delete newAxis6;
        return;
    }

    delete _axis1;
    delete _axis2;
    delete _axis3;
    delete _axis4;
    delete _axis5;
    delete _axis6;

    _axis1 = newAxis1;
    _axis2 = newAxis2;
    _axis3 = newAxis3;
    _axis4 = newAxis4;
    _axis5 = newAxis5;
    _axis6 = newAxis6;
}

void Setup::_updateHomingManager()
{
    // Clear existing groups
    _homingManager.clearGroups();

    // Used unique_ptr instead of raw pointers only to see if it works. Maybe it would be better to use raw pointers but for now it works fine.
    // Create and add groups
    auto group1 = std::make_unique<AxisGroup>();
    group1->addAxis(_axis1);
    group1->addAxis(_axis2);
    group1->addAxis(_axis4);

    auto group2 = std::make_unique<AxisGroup>();
    group2->addAxis(_axis3);

    auto group3 = std::make_unique<AxisGroup>();
    group3->addAxis(_axis5);
    group3->addAxis(_axis6);

    _homingManager.addGroup(std::move(group1));
    _homingManager.addGroup(std::move(group2));
    _homingManager.addGroup(std::move(group3));
}

void Setup::update(const String& jsonString)
{
    if (jsonString.length() > 0)
    {
        _jsonStr = jsonString;
        _validateJson();
        _updateDhParams();
        _updateHomePositions();
        _updateMotionProfiles();
        _updateMotorConfigs();
        _updateKinematics();
        _updateAxisData();
        _updateHomingManager();
    }
}

void Setup::init()
{
    // Runs in setup() of main.cpp
    TS4::begin();
    TimerFactory::attachModule(new TMRModule<0>());

    pinMode(_motorJ1En, OUTPUT);
    pinMode(_motorJ2En, OUTPUT);
    pinMode(_motorJ3En, OUTPUT);
    pinMode(_motorJ4En, OUTPUT);
    pinMode(_motorJ5En, OUTPUT);
    pinMode(_motorJ6En, OUTPUT);

    // digitalWrite(motorJ1En, HIGH);
    // digitalWrite(motorJ2En, HIGH);
    // digitalWrite(motorJ3En, HIGH);
    // digitalWrite(motorJ4En, HIGH);
    // digitalWrite(motorJ5En, HIGH);
    // digitalWrite(motorJ6En, HIGH);

    _limitSwitches.init();
    _serialHandler.setCommandProcessor(&_cmdProcessor);
}