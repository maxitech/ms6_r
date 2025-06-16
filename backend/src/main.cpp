#include "Setup.h" // Where setup is done!

Setup& setupHandler = Setup::getInstance();

void setup()
{
    while (!Serial)
    {
        // wait
    };
    Serial.println("Setup started");
    setupHandler.init();
    Serial.println("Setup done");
}

void loop()
{

    serialHandler.listenForSerial();
    programLoader.run();
    // homingManager.executeHoming();
}
