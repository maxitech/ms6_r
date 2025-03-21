#include "Utils.h"


Utils::Utils() {}


bool Utils::nonBlockingDelay(unsigned long interval, unsigned long &lastUpdateTime) {
    unsigned long currentTime = millis();
    if(currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}