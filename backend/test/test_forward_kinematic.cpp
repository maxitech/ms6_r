#include "Compat.h"
#include <Kinematics.h>
#include <MotorConfig.h>
#include <iostream>
#include <memory>
#include <unity.h>

std::unique_ptr<TS4::Stepper> motor;
std::unique_ptr<MotorConfig>  cfg; // Dummy motor config for testing
std::unique_ptr<Kinematics>   kin; // Dummy kinematics for testing

void setUp(void)
{
    auto motor = std::make_shared<TS4::Stepper>(2, 3); // Dummy motor for testing
    // set stuff up here
    motor->setPosition(1600);
    cfg                              = std::make_unique<MotorConfig>(motor, 200, 16, 20, 40, 2.0f);
    std::vector<MotorConfig> configs = {*cfg};
    kin                              = std::make_unique<Kinematics>(configs, std::vector<DHparam> {});
    cfg->motor->setPosition(1600); // Set motor position to 1600 for testing
}

void tearDown(void)
{
    // clean stuff up here
    cfg.reset();   // Reset the motor config
    kin.reset();   // Reset the kinematics object
    motor.reset(); // Reset the motor object
}

void test_kinematics_initialization(void)
{
    TEST_ASSERT_TRUE_MESSAGE(kin != nullptr, "Kinematics object is null");
    TEST_ASSERT_TRUE_MESSAGE(cfg != nullptr, "Motor config is null");
}

void test_getJointAnglesInRad_returns_vector(void)
{
    if (kin == nullptr)
    {
        TEST_FAIL_MESSAGE("Kinematics object is null");
        return;
    }

    std::vector<float> angles = kin->getJointAnglesInRad();
    std::cout << "Angles vecor size: " << angles.size() << std::endl;

    // Just test that we get a vector with at least one element
    TEST_ASSERT_TRUE_MESSAGE(angles.size() > 0, "Joint angles vector is empty");
    TEST_ASSERT_TRUE_MESSAGE(angles[0] != 0.0f, "Joint angle is zero");
}

void test_getJointAnglesInRad_should_return_correct_angle_in_radians(void)
{

    TEST_ASSERT_TRUE_MESSAGE(kin != nullptr, "Kinematics object should not be null");
    // Given:
    // input
    std::vector<float> actual = kin->getJointAnglesInRad(); // call the function
    // And motorConfig: 200 steps/rev, 16 microsteps, gear ratio = 4.0
    // So: stepsPerRev = 200 * 16 * 4 = 12800
    // Expected angle = (1600 / 12800) * 360 = 45 degrees
    // Expected in radians = 45 * π / 180 = π / 4 ≈ 0.7854
    float expected = M_PI / 4.0f;

    std::cout << "Expected angle in radians: " << expected << std::endl;
    std::cout << "Actual angle in radians: " << actual[0] << std::endl;

    TEST_ASSERT_TRUE_MESSAGE(actual.size() > 0, "Returned joint angles vector is empty");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual[0], "Joint angle should be π/4 radians (45°)");
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_kinematics_initialization);
    RUN_TEST(test_getJointAnglesInRad_returns_vector);
    RUN_TEST(test_getJointAnglesInRad_should_return_correct_angle_in_radians);
    return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
 * For native dev-platform or for some embedded frameworks
 */
int main(void) { return runUnityTests(); }