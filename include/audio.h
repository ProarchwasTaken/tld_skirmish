// audio.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <string>
#include <toml/value.hpp>
#include <vector>

class AudioManager;


class SoundData {
public:
  SoundData(std::string name, uint8_t id, AudioManager *manager);

  std::string name;
  uint8_t id;
  AudioManager *manager;
};


class AudioManager {
public:
  AudioManager();
  ~AudioManager();

  void loadSoundEffects();

private:
  toml::value meta_data;
  std::vector<Sound> sound_effects;
};
