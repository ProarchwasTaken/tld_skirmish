// data/data_enemy.h
#pragma once
#include <cstdint>

struct EnemyMetadata {
  uint8_t enemy_id;
  int8_t screen_side;
  float spawn_time;
};
