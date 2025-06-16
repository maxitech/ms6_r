#include "Setup.h" // Where setup is done!

Setup& setupHandler = Setup::getInstance();

void setup()
{
    while (!Serial)
    {
        // wait
    };
    Serial.println("Setup started");

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

    Serial.println("Setup done");
}

void loop()
{

    serialHandler.listenForSerial();
    programLoader.run();
    // homingManager.executeHoming();
}
