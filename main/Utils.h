#pragma once
#include "Config.h"

extern const byte up = 0;
extern const byte right = 1;
extern const byte down = 2;
extern const byte left = 3;
extern const byte directionsCount = 4;
//extern const byte playerAttack

bool debouncer(unsigned long startTime, unsigned long duration)
{
    if (millis() - startTime > duration) {
        return true;
    }
    return false;
}
