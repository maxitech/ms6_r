#include "Compat.h"
#include <Kinematics.h>
#include <MotorConfig.h>
#include <unity.h>

// Globale Motor- und Config-Objekte
TS4::Stepper* motor1 = nullptr;
TS4::Stepper* motor2 = nullptr;
TS4::Stepper* motor3 = nullptr;
TS4::Stepper* motor4 = nullptr;
TS4::Stepper* motor5 = nullptr;
TS4::Stepper* motor6 = nullptr;

MotorConfig* cfg1 = nullptr;
MotorConfig* cfg2 = nullptr;
MotorConfig* cfg3 = nullptr;
MotorConfig* cfg4 = nullptr;
MotorConfig* cfg5 = nullptr;
MotorConfig* cfg6 = nullptr;

Kinematics* kin = nullptr;

std::vector<DHparam> dhParams = {
    {37.5f, -1.571f, 135.300f},
    {160.0f, 0.0f, 0.0f},
    {-15.0f, 1.571f, 0.0f},
    {0.0f, -1.571f, 138.400f},
    {0.0f, 1.571f, 0.0f},
    {0.0f, 0.0f, 29.270f}};

void setUp(void)
{
    motor1 = new TS4::Stepper(2, 3);
    motor2 = new TS4::Stepper(4, 5);
    motor3 = new TS4::Stepper(6, 7);
    motor4 = new TS4::Stepper(8, 9);
    motor5 = new TS4::Stepper(10, 11);
    motor6 = new TS4::Stepper(12, 13);

    motor1->setPosition(1600);
    motor2->setPosition(-3200);
    motor3->setPosition(6400);
    motor4->setPosition(0);
    motor5->setPosition(0);
    motor6->setPosition(0);

    cfg1 = new MotorConfig(motor1, 200, 16, 20, 40, 2.0f);
    cfg2 = new MotorConfig(motor2, 200, 16, 20, 40, 2.0f);
    cfg3 = new MotorConfig(motor3, 200, 16, 20, 40, 2.0f);
    cfg4 = new MotorConfig(motor4, 200, 16, 20, 40, 2.0f);
    cfg5 = new MotorConfig(motor5, 200, 16, 20, 40, 2.0f);
    cfg6 = new MotorConfig(motor6, 200, 16, 20, 40, 2.0f);

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};

    kin = new Kinematics(configs, std::vector<DHparam> {});
}

void tearDown(void)
{
    delete kin;
    delete cfg1;
    delete cfg2;
    delete cfg3;
    delete cfg4;
    delete cfg5;
    delete cfg6;

    delete motor1;
    delete motor2;
    delete motor3;
    delete motor4;
    delete motor5;
    delete motor6;

    kin  = nullptr;
    cfg1 = cfg2 = cfg3 = cfg4 = cfg5 = cfg6 = nullptr;
    motor1 = motor2 = motor3 = motor4 = motor5 = motor6 = nullptr;
}

void test_kinematics_initialization(void)
{
    TEST_ASSERT_NOT_NULL(kin);
    TEST_ASSERT_NOT_NULL(cfg1);
    TEST_ASSERT_NOT_NULL(motor1);
}

void test_getJointAnglesInRad_returns_vector(void)
{
    TEST_ASSERT_NOT_NULL(kin);
    std::vector<double> angles = kin->getJointAnglesInRad();
    TEST_ASSERT_TRUE(angles.size() > 0);
    TEST_ASSERT_NOT_EQUAL(0.0f, angles[0]);
}

void test_getJointAnglesInRad_should_return_correct_angle_in_radians(void)
{
    TEST_ASSERT_NOT_NULL(kin);
    std::vector<double> actual   = kin->getJointAnglesInRad();
    double              expected = M_PI / 4.0f; // 45°
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual[0]);
}

void test_forwardKinematics_correctPose(void)
{
    cfg1->motor->setPosition(0);

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Pose result = kin->forwardKinematics();

    // Define expected pose values
    float expectedX = 205.170f; // Replace with expected x
    float expectedY = 0.0f;     // Replace with expected y
    float expectedZ = 310.3f;   // Replace with expected z

    float expectedYaw   = -90.0f; // Replace with expected yaw
    float expectedPitch = -90.0f; // Replace with expected pitch
    float expectedRoll  = -90.0f; // Replace with expected roll

    // Validate results
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expectedX, result.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expectedY, result.y);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expectedZ, result.z);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedYaw, result.yaw);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedPitch, result.pitch);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedRoll, result.roll);
}

void test_forwardKinematics_correctPose_w_toolFrame(void)
{
    cfg1->motor->setPosition(0);
    cfg5->motor->setPosition(3200);

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    kin->setToolFrame(10.0f, 20.0f, 30.0f, 45.0f, 60.0f, 90.0f);
    Pose result = kin->forwardKinematics();

    // Expected pose vals with tool frame
    float expectedX = 165.90f;
    float expectedY = 20.0f;
    float expectedZ = 251.030f;

    float expectedYaw   = 135.0f;
    float expectedPitch = -60.0f;
    float expectedRoll  = -90.0f;

    // Validate results
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expectedX, result.x);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expectedY, result.y);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expectedZ, result.z);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedYaw, result.yaw);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedPitch, result.pitch);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedRoll, result.roll);
}

// --------------------------------Inverse Kinematics------------------------------------
void test_inverseKinematics_dynamic(void)
{
    cfg1->motor->setPosition(0);    // Reset for consistent test
    cfg5->motor->setPosition(3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Pose   result = kin->forwardKinematics();
    Angles angles = kin->inverseKinematics(result.x, result.y, result.z, result.roll, result.pitch, result.yaw);
}

void test_inverseKinematics_static_positive(void)
{
    cfg1->motor->setPosition(0);    // Reset for consistent test
    cfg5->motor->setPosition(3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Pose   result    = kin->forwardKinematics();
    Angles anglesPos = kin->inverseKinematics(124.380f, 124.380f, 281.030f, 135.000f, 0.0f, 180.000f); // j1 = 45.0° j5 = 90.0°
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.0f, anglesPos.theta1);
}

void test_inverseKinematics_static_negative(void)
{
    cfg1->motor->setPosition(0);    // Reset for consistent test
    cfg5->motor->setPosition(3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Pose   result    = kin->forwardKinematics();
    Angles anglesNeg = kin->inverseKinematics(124.380f, -124.380f, 281.030f, 135.000f, 0.0f, 180.000f); // j1 = -45.0° j5 = 90.0°
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -45.0f, anglesNeg.theta1);
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    // RUN_TEST(test_kinematics_initialization);
    // RUN_TEST(test_getJointAnglesInRad_returns_vector);
    // RUN_TEST(test_getJointAnglesInRad_should_return_correct_angle_in_radians);
    // RUN_TEST(test_forwardKinematics_correctPose);
    // RUN_TEST(test_forwardKinematics_correctPose_w_toolFrame);
    // RUN_TEST(test_inverseKinematics_dynamic);
    RUN_TEST(test_inverseKinematics_static_positive);
    RUN_TEST(test_inverseKinematics_static_negative);
    return UNITY_END();
}

/**
 * For native dev-platform or for some embedded frameworks
 */

int main(void)
{
    return runUnityTests();
}

/**
 * For Arduino framework
 */
void setup()
{
    // delay(2000);

    UNITY_BEGIN();
    runUnityTests();
    UNITY_END();
}

void loop()
{
}
