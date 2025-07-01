#include "Setup.h" // Where setup is done!

Setup& setupHandler = Setup::getInstance();

void setup()
{
    while (!Serial)
    {
        // wait
    };
    setupHandler.init();
}

void loop()
{
    setupHandler.getSerialHandler().listenForSerial();
    setupHandler.getProgramLoader().run();
}
