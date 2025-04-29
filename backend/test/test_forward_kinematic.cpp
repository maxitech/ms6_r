#include "Compat.h"
#include <Kinematics.h>
#include <MotorConfig.h>
#include <memory>
#include <unity.h>

std::unique_ptr<MotorConfig> cfg;
std::unique_ptr<Kinematics>  kin;
Stepper                      motor;

void setUp(void)
{
    // set stuff up here
    cfg                              = std::make_unique<MotorConfig>(motor, 200, 16, 20, 40, 2.0f);
    std::vector<MotorConfig> configs = {*cfg};
    kin                              = std::make_unique<Kinematics>(configs, std::vector<DHparam> {});
}

void tearDown(void)
{
    // clean stuff up here
}

void test_function_should_doBlahAndBlah(void)
{
    // test stuff
}

void test_function_should_doAlsoDoBlah(void)
{
    // more test stuff
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_function_should_doBlahAndBlah);
    RUN_TEST(test_function_should_doAlsoDoBlah);
    return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
 * For native dev-platform or for some embedded frameworks
 */
int main(void) { return runUnityTests(); }