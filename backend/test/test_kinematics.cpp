#include "Compat.h"
#include <Kinematics.h>
#include <MotorConfig.h>
#include <unity.h>

// Global variables
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

int TEST_HOME_POS_J1 = 40'000;
int TEST_HOME_POS_J2 = 55'000;
int TEST_HOME_POS_J3 = -20'000;
int TEST_HOME_POS_J4 = -24'000;
int TEST_HOME_POS_J5 = -22'000;
int TEST_HOME_POS_J6 = -6'400;

// correct in theory => problems with kinemaic => atm no idea why => keep "old" functional dh params
// std::array<DHparam, 6> dhParams = {
//     DHparam {37.5, -1.571, 135.300, 0.0},
//     DHparam {160.0, 3.142, 0.0, -90.0},
//     DHparam {-15.0, 1.571, 0.0, 180.0},
//     DHparam {0.0, -1.571, -138.400, 0.0},
//     DHparam {0.0, 1.571, 0.0, 0.0},
//     DHparam {0.0, 3.142, -29.270, 0.0}};

std::array<DHparam, 6> dhParams = {
    DHparam {37.5, -1.571, 135.300, 0.0},
    DHparam {160.0, 0.0, 0.0, -90.0},
    DHparam {-15.0, 1.571, 0.0, 180.0},
    DHparam {0.0, -1.571, 138.400, 0.0},
    DHparam {0.0, 1.571, 0.0, 0.0},
    DHparam {0.0, 0.0, 29.270, 0.0}};

std::vector<MotorConfig*> motorConfigs;

void setUp(void)
{
    motor1 = new TS4::Stepper(2, 3);
    motor2 = new TS4::Stepper(4, 5);
    motor3 = new TS4::Stepper(6, 7);
    motor4 = new TS4::Stepper(8, 9);
    motor5 = new TS4::Stepper(10, 11);
    motor6 = new TS4::Stepper(12, 13);

    motor1->setPosition(20000);  // 45° in steps
    motor2->setPosition(122000); // 45° in steps
    motor3->setPosition(20000);  // 45° in steps
    motor4->setPosition(TEST_HOME_POS_J4);
    motor5->setPosition(TEST_HOME_POS_J5);
    motor6->setPosition(TEST_HOME_POS_J6);

    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, TEST_HOME_POS_J4, -180, 180, 400, 64, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, TEST_HOME_POS_J5, -180, 180, 200, 64, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, TEST_HOME_POS_J6, -180, 180, 200, 64, 1.0f, 1.0f, 1.0f));

    // kin = new Kinematics(motorConfigs, std::vector<DHparam> {});
    kin = new Kinematics(motorConfigs, dhParams);
}

void tearDown(void)
{
    delete kin;

    delete motor1;
    delete motor2;
    delete motor3;
    delete motor4;
    delete motor5;
    delete motor6;

    for (auto cfg : motorConfigs)
    {
        delete cfg;
    }

    kin    = nullptr;
    motor1 = motor2 = motor3 = motor4 = motor5 = motor6 = nullptr;
}

// --------------------------------Forward Kinematics------------------------------------

void test_kinematics_initialization(void)
{
    TEST_ASSERT_NOT_NULL(kin);
    for (const auto& cfg : motorConfigs)
    {
        TEST_ASSERT_NOT_NULL(cfg);
    }
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
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, TEST_HOME_POS_J4, -180, 180, 400, 4, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, TEST_HOME_POS_J5, -180, 180, 200, 4, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, TEST_HOME_POS_J6, -180, 180, 200, 4, 1.0f, 1.0f, 1.0f));

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
    motor1->setPosition(10000); // 0° in steps
    motor2->setPosition(10000); // 0° in steps
    motor3->setPosition(10000); // 0° in steps
    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

    Pose result = kin->forwardKinematics();

    // Define expected pose values
    float expectedX = 205.170f; // Replace with expected x
    float expectedY = 0.0f;     // Replace with expected y
    float expectedZ = 310.3f;   // Replace with expected z

    float expectedYaw   = -90.0f; // Replace with expected yaw
    float expectedPitch = -90.0f; // Replace with expected pitch
    float expectedRoll  = -90.0f; // Replace with expected roll

    // Validate results
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expectedX, result.x);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expectedY, result.y);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expectedZ, result.z);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedYaw, result.yaw);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedPitch, result.pitch);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, expectedRoll, result.roll);
}

void test_forwardKinematics_correctPose_w_toolFrame(void)
{
    motor1->setPosition(10000); // 0° in steps
    motor2->setPosition(10000); // 0° in steps
    motor3->setPosition(10000); // 0° in steps
    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

    kin->setToolFrame(10.0f, 20.0f, 30.0f, 45.0f, 60.0f, 90.0f);
    Pose result = kin->forwardKinematics();

    // Expected pose vals with tool frame
    float expectedX = 235.170f;
    float expectedY = 20.0f;
    float expectedZ = 300.30f;

    float expectedYaw   = 157.792f;
    float expectedPitch = 20.0f;
    float expectedRoll  = -139.107f;

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
    motor1->setPosition(10000); // 0° in steps
    motor2->setPosition(10000); // 0° in steps
    motor3->setPosition(10000); // 0° in steps
    motor5->setPosition(16400); // 90° in steps

    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, TEST_HOME_POS_J4, -180, 180, 400, 64, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, 10000, -180, 180, 200, 64, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, TEST_HOME_POS_J6, -180, 180, 200, 64, 1.0f, 1.0f, 1.0f));

    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

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
    // TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta5, angles.theta5); // j5 = 90.0°
    TEST_ASSERT_TRUE(fabs(angles.theta5 - 90.0f) < 0.1f || fabs(angles.theta5 + 90.0f) < 0.1f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, original.theta6, angles.theta6); // j6 = 0.0°
}

void test_inverseKinematics_static_j5_positive(void)
{
    motor1->setPosition(20000);  // 45° in steps
    motor2->setPosition(92133);  // 33° in steps
    motor3->setPosition(15111);  // 23° in steps
    motor4->setPosition(55333);  // 170° in steps
    motor5->setPosition(16400);  // 90° in steps
    motor6->setPosition(13.947); // 111° in steps

    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, 10000, -180, 180, 400, 64, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, 10000, -180, 180, 200, 64, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, 10000, -180, 180, 200, 64, 1.0f, 1.0f, 1.0f));

    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

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
    motor1->setPosition(20000);  // 45° in steps
    motor2->setPosition(92133);  // 33° in steps
    motor3->setPosition(15111);  // 23° in steps
    motor4->setPosition(55333);  // 170° in steps
    motor5->setPosition(3600);   // -90° in steps
    motor6->setPosition(13.947); // 111° in steps

    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, 10000, -180, 180, 400, 64, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, 10000, -180, 180, 200, 64, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, 10000, -180, 180, 200, 64, 1.0f, 1.0f, 1.0f));

    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

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
    motor1->setPosition(20000);  // 45° in steps
    motor2->setPosition(92133);  // 33° in steps
    motor3->setPosition(15111);  // 23° in steps
    motor4->setPosition(55333);  // 170° in steps
    motor5->setPosition(16400);  // 90° in steps
    motor6->setPosition(13.947); // 111° in steps

    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, 10000, -180, 180, 400, 64, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, 10000, -180, 180, 200, 64, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, 10000, -180, 180, 200, 64, 1.0f, 1.0f, 1.0f));
    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

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
    motor1->setPosition(20000);  // 45° in steps
    motor2->setPosition(92133);  // 33° in steps
    motor3->setPosition(15111);  // 23° in steps
    motor4->setPosition(55333);  // 170° in steps
    motor5->setPosition(3600);   // -90° in steps
    motor6->setPosition(13.947); // 111° in steps

    motorConfigs.clear();
    motorConfigs.push_back(new MotorConfig(motor1, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor2, 10000, -180, 180, 200, 64, 16.0f, 80.0f, 14.0f));
    motorConfigs.push_back(new MotorConfig(motor3, 10000, -180, 180, 200, 64, 16.0f, 100.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor4, 10000, -180, 180, 400, 64, 16.0f, 60.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor5, 10000, -180, 180, 200, 64, 16.0f, 32.0f, 1.0f));
    motorConfigs.push_back(new MotorConfig(motor6, 10000, -180, 180, 200, 64, 1.0f, 1.0f, 1.0f));
    delete kin;
    kin = new Kinematics(motorConfigs, dhParams);

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
    RUN_TEST(test_getJointAnglesInRadOrDeg_returns_vector);
    RUN_TEST(test_getJointAnglesInRadOrDeg_should_return_correct_angle_in_degree);
    RUN_TEST(test_getJointAnglesInRadOrDeg_should_return_correct_angle_in_radian);
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
