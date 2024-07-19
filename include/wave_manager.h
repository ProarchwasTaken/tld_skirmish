// wave_manager.h
#pragma once
#include <cstdint>
#include <vector>
#include <toml/value.hpp>
#include "base/generics.h"


struct EnemyMetadata {
  EnemyMetadata(uint8_t enemy_id, int8_t screen_side, float spawn_time);

  uint8_t enemy_id;
  int8_t screen_side;
  float spawn_time;
};

#define ENEMY_GHOUL 0;


class WaveManager {
public:
  WaveManager(combatant_list &enemies);

  void startWave(uint8_t difficulty);
  std::vector<toml::value> waveSearch(uint8_t difficulty);
  void assignWave(toml::value wave);

  void waveSequence();

  uint16_t wave_timer = 0;
private:
  combatant_list *enemies;
  toml::value wave_metadata;

  std::vector<EnemyMetadata> enemy_queue;

  float wave_timestamp = 0;
};
