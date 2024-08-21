// sys_wave_manager.h
#pragma once
#include <cstdint>
#include <vector>
#include <list>
#include <toml/value.hpp>
#include "base/generics.h"
#include "char_player.h"

#define ENEMY_GHOUL 0
#define ENEMY_WRETCH 1


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

  /* Basically reloads the external toml file that's used for a majority
   * of this classes methods. A debug method that's only called through 
   * the use of a keybind. Useful for making changes to the toml file
   * without needing to reset the game.*/
  void reloadWaveData();

  /* The main method for starting a random wave of a specific difficulty.
   * Makes use of other helper methods to get this done, and it's most
   * commonly called during the GameplayScene.*/
  void startWave(uint8_t difficulty);

  /* Searches for every available wave of a certain difficulty, and
   * returns a list of them. Also makes sure not to add any waves that
   * were already used in the WaveManager's lifetime, and it returns
   * an empty list if no valid waves were found.*/
  std::vector<toml::value> waveSearch(uint8_t difficulty);

  /* This method is used for testing specific waves more often than not.
   * Has similar functionality to startWave.*/
  void startWaveByID(int wave_id);

  /* The Coup de grace towards starting waves, and it's functions as a
   * helper method for the two startWave methods. It basically reads the
   * wave data given to it, and serializes it to be used by waveSequence.
   * More specifically, the wave's EnemyMetadata.*/
  void assignWave(toml::value wave);

  /* Responsible for spawning enemies for as long as the queue is not
   * empty. Should be called once every time. How the method reads the
   * enemy_queue to know what enemy to spawn, and when to spawn it.*/
  void waveSequence();

  /* Functionally acts as a wrapper to spawning new enemies. The 
   * parameters needed to call this method are usually obtained from
   * EnemyMetadata. This could also be used for debug purposes like
   * testing new enemies.*/
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
