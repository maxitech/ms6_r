#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class Utils
{
public:
    Utils();
    static bool nonBlockingDelay(unsigned long interval, unsigned long& lastUpdateTime);
};

#endif // UTILS_H
