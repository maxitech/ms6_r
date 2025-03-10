#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <Arduino.h>
#include "Commands.h"

void readSerialInput();
void handleNewCommand();

#endif
