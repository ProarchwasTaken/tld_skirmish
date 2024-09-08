// utils/ai_behavior.cpp
#include <raylib.h>
#include <cstdint>
#include "utils_ai_behavior.h"
#include "globals.h"


void AIBehavior::tickPatience(uint8_t &patience, float tick_timestamp, 
                              float tick_rate)
{
  float time_elapsed = CURRENT_TIME - tick_timestamp;

  bool should_tick = patience != 0 && time_elapsed >= tick_rate;
  if (should_tick) {
    patience--;
    tick_timestamp = CURRENT_TIME;
  }
}
