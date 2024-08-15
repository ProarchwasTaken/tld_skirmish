// sys_wave_manager.h
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


/* The WaveManager is primarily responsible for handling the spawning of
 * the game's enemies in accordance to the current wave. Data about each
 * wave of enemies in the game is retrieved from an external toml file
 * which used often in the majority of the class's methods. If you want to
 * add addition waves, that's where you'd look.*/
class WaveManager {
public:
  WaveManager(PlayerCharacter &player, combatant_list &enemies);
  ~WaveManager();

  void startWave(uint8_t difficulty);
  void startWaveByID(int wave_id);

  std::vector<toml::value> waveSearch(uint8_t difficulty);
  void assignWave(toml::value wave);

  void waveSequence();
  void spawnEnemy(uint8_t enemy_id, int8_t spawn_side);

  uint16_t wave_timer = 0;
  std::list<EnemyMetadata> enemy_queue;
private:
  PlayerCharacter *player;
  combatant_list *enemies;

  toml::value wave_metadata;

  std::vector<int> used_waves;
  float wave_timestamp = 0;
};
