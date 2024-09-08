// utils_ai_behavior.h
#pragma once
#include <cstdint>


/* Functions that are related to common AI behavior for enemies, bosses,
 * and any character that's not directly controlled by the player.*/
namespace AIBehavior {
  /* Patience is a common variable used by enemies and the like to 
   * determine whether or not they should act depending on it's value.
   * By default, this function is for automatically decrementing that 
   * value 20 times per second. Stopping at 0.*/
  void tickPatience(uint8_t &patience, float tick_timestamp, 
                    float tick_rate = 0.05);
}

