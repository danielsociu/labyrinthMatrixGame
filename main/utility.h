#include "config.h"

bool debouncer(unsigned long startTime, unsigned long duration)
{
  if (millis() - startTime > duration) {
    return true;
  }
  return false;
}
