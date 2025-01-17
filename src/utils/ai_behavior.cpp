// utils/ai_behavior.cpp
#include <raylib.h>
#include <cstdint>
#include <triblib/globals.h>
#include "utils/ai_behavior.h"


void AIBehavior::tickPatience(uint8_t &patience, float &tick_timestamp, 
                              const float tick_rate)
{
  const float time_elapsed = CURRENT_TIME - tick_timestamp;

  bool should_tick = patience != 0 && time_elapsed >= tick_rate;
  if (should_tick) {
    patience--;
    tick_timestamp = CURRENT_TIME;
  }
}
