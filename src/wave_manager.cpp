// wave_manager.cpp
#include <raylib.h>
#include <cstdint>
#include <toml/parser.hpp>
#include <toml/value.hpp>
#include <vector>
#include <random>
#include <memory>
#include <string>
#include "base/generics.h"
#include "globals.h"
#include "enemy_ghoul.h"
#include "wave_manager.h"
#include <plog/Log.h>

using std::vector, std::uniform_int_distribution, std::make_shared, 
std::string;


EnemyMetadata::EnemyMetadata(uint8_t enemy_id, int8_t screen_side, 
                             float spawn_time)
{
  this->enemy_id = enemy_id;
  this->screen_side = screen_side;
  this->spawn_time = spawn_time;
}


WaveManager::WaveManager(PlayerCharacter &player, combatant_list &enemies) 
{
  this->player = &player;
  this->enemies = &enemies;

  wave_metadata = toml::parse("data/enemy_waves.toml")["waves"];
  PLOGI << "Initialized Wave Manager";
}

WaveManager::~WaveManager() {
  enemy_queue.clear();
}

void WaveManager::startWave(uint8_t difficulty) {
  vector<toml::value> waves_found;
  bool waves_not_found;

  PLOGD << "Getting all waves associated with difficulty: " <<
    int(difficulty);
  do {
    if (difficulty == 0) {
      PLOGE << "Absolutely no valid waves were found!";
      throw;
    }

    waves_found = waveSearch(difficulty);
    waves_not_found = waves_found.size() == 0;

    if (waves_not_found) {
      difficulty--;
    } 

  } while (waves_not_found);

  PLOGD << "Found waves of identical difficulty level.";
  PLOGD << "Proceeding to choose random wave.";
  int count = waves_found.size();
  uniform_int_distribution<int> range(0, count - 1);

  int wave_index = range(RNG::generator);
  toml::value chosen_wave = waves_found[wave_index];
  string wave_name = chosen_wave["name"].as_string();

  PLOGI << "Now starting wave: " << wave_name;
  assignWave(chosen_wave);
  wave_timestamp = GetTime();
}

vector<toml::value> WaveManager::waveSearch(uint8_t difficulty) {
  vector<toml::value> waves_found;
  int wave_count = wave_metadata.size();

  for (int index = 0; index < wave_count; index++) {
    toml::value wave = wave_metadata[index];
    int wave_difficulty = wave["difficulty"].as_integer();

    if (difficulty == wave_difficulty) {
      waves_found.push_back(wave);
    }
  }

  return waves_found;
}

void WaveManager::assignWave(toml::value wave) {
  wave_timer = wave["time_limit"].as_integer();
  int enemy_count = wave["enemies"].size();

  if (enemy_count == 0) {
    PLOGW << "There are no enemies in this wave!";
    return;
  }

  for (int index = 0; index < enemy_count; index++) {
    toml::value enemy_data = wave["enemies"][index];

    uint8_t id = enemy_data["enemy_id"].as_integer();
    int8_t screen_side = enemy_data["screen_side"].as_integer();
    float spawn_time = enemy_data["spawn_time"].as_floating();

    enemy_queue.push_back(EnemyMetadata(id, screen_side, spawn_time));
  }
}

void WaveManager::waveSequence() {
  if (enemy_queue.size() == 0) {
    return;
  }

  EnemyMetadata *enemy = &enemy_queue.front();
  float time_elapsed = GetTime() - wave_timestamp;

  if (time_elapsed >= enemy->spawn_time) {
    uint8_t enemy_id = enemy->enemy_id;
    int8_t spawn_side = enemy->screen_side;

    spawnEnemy(enemy_id, spawn_side);
    enemy_queue.pop_front();
  }
}

void WaveManager::spawnEnemy(uint8_t enemy_id, int8_t spawn_side) {
  PLOGI << "Attempting to spawn enemy associated with ID: " << 
    int(enemy_id);

  float spawn_x = 512 * spawn_side;
  Vector2 position = {spawn_x, 152};

  switch (enemy_id) {
    case ENEMY_GHOUL: {
      enemies->push_back(make_shared<GhoulEnemy>(*player, position));
      break;
    }
    default: {
      PLOGE << "There is no enemy associated with specified ID!";
    }
  }
}
