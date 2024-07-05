// utils/ai_behavior.cpp
#include <raylib.h>
#include <cstdint>
#include "utils.h"


void AIBehavior::tickPatience(uint8_t &patience, float tick_timestamp, 
                              float tick_rate)
{
  float time_elapsed = GetTime() - tick_timestamp;

  bool should_tick = patience != 0 && time_elapsed >= tick_rate;
  if (should_tick) {
    patience--;
    time_elapsed = GetTime();
  }
}
