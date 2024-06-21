// audio.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <string>
#include <toml/value.hpp>
#include <vector>

class AudioManager;


/* Sound meta data is meant to store infomation about a specific sound
 * effect load into the game, and make it easier for it to retrieve.
 * This kind of stuff is typically meant to be used by utility 
 * functions.*/
class SoundMetaData {
public:
  SoundMetaData(std::string name, uint8_t id, AudioManager *manager);

  std::string name;
  uint8_t id;
  AudioManager *manager;
};


/* The Audio Manager where the game's sound effects are loaded and stored.
 * The same goes for the game's music as well. This is all done through
 * a usage of a TOML file located in the audio directory.*/
class AudioManager {
public:
  AudioManager();
  ~AudioManager();

  void loadSoundEffects();

  std::vector<Sound> sound_effects;
private:
  toml::value meta_data;
};
