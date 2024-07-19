// wave_manager.h
#pragma once
#include <cstdint>
#include <vector>
#include <list>
#include <toml/value.hpp>
#include "base/generics.h"
#include "char_player.h"

#define ENEMY_GHOUL 0


struct EnemyMetadata {
  EnemyMetadata(uint8_t enemy_id, int8_t screen_side, float spawn_time);

  uint8_t enemy_id;
  int8_t screen_side;
  float spawn_time;
};


class WaveManager {
public:
  WaveManager(PlayerCharacter &player, combatant_list &enemies);

  void startWave(uint8_t difficulty);
  std::vector<toml::value> waveSearch(uint8_t difficulty);
  void assignWave(toml::value wave);

  void waveSequence();
  void spawnEnemy(uint8_t enemy_id, int8_t spawn_side);

  uint16_t wave_timer = 0;
private:
  PlayerCharacter *player;
  combatant_list *enemies;

  toml::value wave_metadata;
  std::list<EnemyMetadata> enemy_queue;

  float wave_timestamp = 0;
};
