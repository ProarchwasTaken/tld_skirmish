// sys_audio.h
#pragma once
#include <raylib.h>
#include <toml/value.hpp>
#include <vector>
#include <tuple>


/* The Audio Manager where the game's sound effects are loaded and stored.
 * The same goes for the game's music as well. This is all done through
 * a usage of a TOML file located in the audio directory.*/
class AudioManager {
public:
  AudioManager();
  ~AudioManager();

  void loadSoundEffects();
  std::tuple<float, float> getPitchValues(toml::value sound_data);

private:
  toml::value meta_data;
  std::vector<Sound> sound_effects;
};
