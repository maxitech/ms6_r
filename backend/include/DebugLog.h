#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <Arduino.h>

enum LogLevel
{
    LOG_NONE,
    LOG_WARN,
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
};

// Enable/disable debug messages globally
#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 1 // 0 = disable, 1 = enable
#endif

class DebugLog
{
public:
    static void log(LogLevel level, const String& msg)
    {
#if DEBUG_ENABLED
        switch (level)
        {
        case LOG_WARN:
            Serial.print("[WARN] ");
            break;
        case LOG_ERROR:
            Serial.print("[ERR] ");
            break;
        case LOG_INFO:
            Serial.print("[INFO] ");
            break;
        case LOG_DEBUG:
            Serial.print("[DBG] ");
            break;
        default:
            break;
        }
        Serial.println(msg);
#endif
    }
};

#endif // DEBUGLOG_H