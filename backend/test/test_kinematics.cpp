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

// --------------------------------Forward Kinematics------------------------------------

void test_kinematics_initialization(void)
{
    TEST_ASSERT_NOT_NULL(kin);
    TEST_ASSERT_NOT_NULL(cfg1);
    TEST_ASSERT_NOT_NULL(motor1);
}

void test_getJointAnglesInRadOrDeg_returns_vector(void)
{
    TEST_ASSERT_NOT_NULL(kin);
    std::vector<double> anglesRad = kin->getJointAnglesInRadOrDeg(1); // 1 for radians
    std::vector<double> anglesDeg = kin->getJointAnglesInRadOrDeg(0); // 0 for degrees
    TEST_ASSERT_TRUE(anglesRad.size() > 0);
    TEST_ASSERT_TRUE(anglesDeg.size() > 0);
    TEST_ASSERT_TRUE(anglesRad[0] > 0);
    TEST_ASSERT_TRUE(anglesDeg[0] > 0);
}

void test_getJointAnglesInRadOrDeg_should_return_correct_angle_in_degree(void)
{
    motor1->setPosition(20000);  // 45° in steps
    motor2->setPosition(122000); // 45° in steps
    motor3->setPosition(20000);  // 45° in steps
    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig {motor1, 10000, 200, 64, 16.0f, 100.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor2, 10000, 200, 64, 16.0f, 80.0f, 14.0f});
    motorConfigs.push_back(new MotorConfig {motor3, 10000, 200, 64, 16.0f, 100.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor4, TEST_HOME_POS_J4, 400, 64, 16.0f, 60.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor5, TEST_HOME_POS_J5, 200, 64, 16.0f, 32.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor6, TEST_HOME_POS_J6, 200, 64, 1.0f, 1.0f, 1.0f});

    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);
    TEST_ASSERT_NOT_NULL(kin);
    std::vector<double> actual = kin->getJointAnglesInRadOrDeg(0); // 0 for degrees
    // Debug output
    // for (size_t i = 0; i < actual.size(); ++i)
    // {
    //     std::cout << "Joint " << (i + 1) << " angle in deg: " << actual[i] << std::endl;
    // }
    double expected = 45.0; // 45°
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual[1]); // Assuming both motors are set to 45°
}

void test_getJointAnglesInRadOrDeg_should_return_correct_angle_in_radian(void)
{
    motor1->setPosition(30000);  // 1.57 rad (90°) in steps
    motor2->setPosition(234000); // 1.57 rad (90°) in steps
    motor3->setPosition(30000);  // 1.57 rad (90°) in steps
    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig {motor1, 10000, 200, 64, 16.0f, 100.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor2, 10000, 200, 64, 16.0f, 80.0f, 14.0f});
    motorConfigs.push_back(new MotorConfig {motor3, 10000, 200, 64, 16.0f, 100.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor4, TEST_HOME_POS_J4, 400, 4, 16.0f, 60.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor5, TEST_HOME_POS_J5, 200, 4, 16.0f, 32.0f, 1.0f});
    motorConfigs.push_back(new MotorConfig {motor6, TEST_HOME_POS_J6, 200, 4, 1.0f, 1.0f, 1.0f});

    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);
    TEST_ASSERT_NOT_NULL(kin);
    std::vector<double> actual = kin->getJointAnglesInRadOrDeg(1); // 1 for radians
    // Debug output
    // for (size_t i = 0; i < actual.size(); ++i)
    // {
    //     std::cout << "Joint " << (i + 1) << " angle in radian: " << actual[i] << std::endl;
    // }
    double expected = 1.57; // 90° in radians
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual[2]);
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

void test_fk_to_ik_angle_reconstruction()
{
    cfg1->motor->setPosition(0);
    cfg5->motor->setPosition(3200);

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Angles original = {0, 0, 0, 0, 90.0f, 0};
    Pose   fkPose   = kin->forwardKinematics();

    Angles angles = kin->inverseKinematics(fkPose.x, fkPose.y, fkPose.z, fkPose.yaw, fkPose.pitch, fkPose.roll);

    // Debug output
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "FK Pose: " << fkPose.x << ", " << fkPose.y << ", " << fkPose.z << ", "
    //           << fkPose.yaw << ", " << fkPose.pitch << ", " << fkPose.roll << std::endl;
    // std::cout << "IK Angles: ";
    // std::cout << "  " << angles.theta1 << ", " << angles.theta2 << ", " << angles.theta3 << ", "
    //           << angles.theta4 << ", " << angles.theta5 << ", " << angles.theta6 << std::endl;

    // std::cout << "Original: " << original.theta1 << ", " << original.theta2 << ", " << original.theta3 << ", "
    //           << original.theta4 << ", " << original.theta5 << ", " << original.theta6 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;

    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta1, angles.theta1); // j1 = 0.0°
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta2, angles.theta2); // j2 = 0.0°
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta3, angles.theta3); // j3 = 0.0°
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta4, angles.theta4); // j4 = 0.0°
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta5, angles.theta5); // j5 = 90.0°
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta6, angles.theta6); // j6 = 0.0°
}

void test_inverseKinematics_static_j5_positive(void)
{
    cfg5->motor->setPosition(3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Angles original = {45.0f, 33.0f, 23.0f, 170.0f, 90.0f, 111.0f}; // Represents the expected angles
    Angles angles   = kin->inverseKinematics(164.957f, 172.145f, 179.255f, -24.991f, 11.914f, -55.749f);

    // Debug output
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "IK Angles: ";
    // std::cout << "  " << angles.theta1 << ", " << angles.theta2 << ", " << angles.theta3 << ", "
    //           << angles.theta4 << ", " << angles.theta5 << ", " << angles.theta6 << std::endl;
    // std::cout << "Original: " << original.theta1 << ", " << original.theta2 << ", " << original.theta3 << ", "
    //           << original.theta4 << ", " << original.theta5 << ", " << original.theta6 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;

    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta1, angles.theta1);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta2, angles.theta2);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta3, angles.theta3);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta4, angles.theta4);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta5, angles.theta5);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta6, angles.theta6);
}

void test_inverseKinematics_static_j5_negative(void)
{
    cfg1->motor->setPosition(0);     // Reset for consistent test
    cfg5->motor->setPosition(-3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Angles original = {45.0f, 33.0f, 23.0f, 170.0f, -90.0f, 111.0f}; // Represents the expected angles
    Angles angles   = kin->inverseKinematics(138.349f, 131.161f, 147.017f, -49.106f, -22.815f, 126.687f);

    // Debug output
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "IK Angles: ";
    // std::cout << "  " << angles.theta1 << ", " << angles.theta2 << ", " << angles.theta3 << ", "
    //           << angles.theta4 << ", " << angles.theta5 << ", " << angles.theta6 << std::endl;
    // std::cout << "Original: " << original.theta1 << ", " << original.theta2 << ", " << original.theta3 << ", "
    //           << original.theta4 << ", " << original.theta5 << ", " << original.theta6 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;

    // test angles are equal to position in space - which are put in the ik function
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta1, angles.theta1);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta2, angles.theta2);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta3, angles.theta3);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta4, angles.theta4);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta5, angles.theta5);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta6, angles.theta6);
}

void test_inverseKinematics_static_j5_positive_w_toolFrame(void)
{
    cfg5->motor->setPosition(3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Angles original = {45.0f, 33.0f, 23.0f, 170.0f, 90.0f, 111.0f}; // Represents the expected angles
    kin->setToolFrame(0.0f, 0.0f, 23.5f, 0.0f, 0.0f, 0.0f);
    Angles angles = kin->inverseKinematics(175.639f, 188.598f, 192.197f, -24.991f, 11.914f, -55.749f);

    // Debug output
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "IK Angles: ";
    // std::cout << "  " << angles.theta1 << ", " << angles.theta2 << ", " << angles.theta3 << ", "
    //           << angles.theta4 << ", " << angles.theta5 << ", " << angles.theta6 << std::endl;
    // std::cout << "Original: " << original.theta1 << ", " << original.theta2 << ", " << original.theta3 << ", "
    //           << original.theta4 << ", " << original.theta5 << ", " << original.theta6 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;

    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta1, angles.theta1);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta2, angles.theta2);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta3, angles.theta3);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta4, angles.theta4);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta5, angles.theta5);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta6, angles.theta6);
}

void test_inverseKinematics_static_j5_negative_w_toolFrame(void)
{
    cfg5->motor->setPosition(-3200); // Reset for consistent test

    std::vector<MotorConfig> configs = {*cfg1, *cfg2, *cfg3, *cfg4, *cfg5, *cfg6};
    delete kin;
    kin = new Kinematics(configs, dhParams);

    Angles original = {45.0f, 33.0f, 23.0f, 170.0f, -90.0f, 111.0f}; // Represents the expected angles
    kin->setToolFrame(0.0f, 0.0f, 23.5f, 0.0f, 0.0f, 0.0f);
    Angles angles = kin->inverseKinematics(127.668f, 114.709f, 134.076f, -49.106f, -22.815f, 126.687f);

    // Debug output
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << std::fixed << std::setprecision(6);
    // std::cout << "IK Angles: ";
    // std::cout << "  " << angles.theta1 << ", " << angles.theta2 << ", " << angles.theta3 << ", "
    //           << angles.theta4 << ", " << angles.theta5 << ", " << angles.theta6 << std::endl;
    // std::cout << "Original: " << original.theta1 << ", " << original.theta2 << ", " << original.theta3 << ", "
    //           << original.theta4 << ", " << original.theta5 << ", " << original.theta6 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;

    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta1, angles.theta1);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta2, angles.theta2);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta3, angles.theta3);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta4, angles.theta4);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta5, angles.theta5);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta6, angles.theta6);
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_kinematics_initialization);
    RUN_TEST(test_getJointAnglesInRad_returns_vector);
    RUN_TEST(test_getJointAnglesInRad_should_return_correct_angle_in_radians);
    RUN_TEST(test_forwardKinematics_correctPose);
    RUN_TEST(test_forwardKinematics_correctPose_w_toolFrame);
    RUN_TEST(test_fk_to_ik_angle_reconstruction);
    RUN_TEST(test_inverseKinematics_static_j5_positive);
    RUN_TEST(test_inverseKinematics_static_j5_negative);
    RUN_TEST(test_inverseKinematics_static_j5_positive_w_toolFrame);
    RUN_TEST(test_inverseKinematics_static_j5_negative_w_toolFrame);
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
