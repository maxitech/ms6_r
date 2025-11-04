#ifndef COMPAT_H
#define COMPAT_H

#if __has_include(<ArduinoEigenDense.h>)
#include <ArduinoEigenDense.h>
#else
#include <Eigen/Dense>
#endif

#ifdef TEENSY41
// #include "teensystep4.h"
#include <AccelStepper.h>
#include <Arduino.h>

#else
#include "d_teensystep4.h"
#include <iomanip>
#include <iostream>
#endif

// Cross-Platform Logging
inline void logMessage(const char* message)
{
#if defined(UNIT_TEST) && !defined(TEENSY41)
    std::cout << "[LOG] " << message << std::endl;
#else
    Serial.println(message);
#endif
}

#endif // COMPAT_H
