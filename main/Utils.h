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

// free memory checker
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
