// audio.h
#include <cstdint>
#include <raylib.h>
#include <string>
#include <toml/get.hpp>
#include <toml/parser.hpp>
#include <toml/value.hpp>
#include <tuple>
#include "globals.h"
#include "audio.h"
#include <plog/Log.h>

using std::string, std::tuple, std::make_tuple, std::tie;


SoundMetaData::SoundMetaData(string name, uint8_t id, 
                             AudioManager *manager, bool random_pitch,
                             float min_pitch, float max_pitch) 
{
  this->name = name;
  this->id = id;

  this->random_pitch = random_pitch;
  this->min_pitch = min_pitch;
  this->max_pitch = max_pitch;

  this->manager = manager;
  PLOGI << "Saved sound data for: " << name;
}


AudioManager::AudioManager() {
  meta_data = toml::parse("audio/audio_data.toml");
  loadSoundEffects();
}

AudioManager::~AudioManager() {
  PLOGV << "Unloading all sound effects...";
  for (Sound sfx : sound_effects) {
    UnloadSound(sfx);
  }

  audio::sfx_metadata.clear();
  sound_effects.clear();
  PLOGV << "Sound effects have been unloaded.";
}

void AudioManager::loadSoundEffects() {
  PLOGV << "Loading sound effects.";
  int count = meta_data["sfx"].size();
  PLOGI << "Sound effect detected: " << count;

  for (int index = 0; index < count; index++) {
    toml::value data = meta_data["sfx"][index];

    string sound_name = data["name"].as_string(); 
    string sound_path = data["path"].as_string();
    PLOGI << "Attempting to set up sound effect: " << sound_name;

    bool use_random_pitch = toml::find_or(data, "pitch_random", false);

    float min_pitch, max_pitch = 1.0;
    if (use_random_pitch) {
      PLOGI << "Sound uses random pitch. Attempting to get pitch values.";
      tie(min_pitch, max_pitch) = getPitchValues(data);
    }

    Sound sound = LoadSound(sound_path.c_str());
    sound_effects.push_back(sound);

    audio::sfx_metadata.push_back(
      SoundMetaData(sound_name, index, this, use_random_pitch, min_pitch,
                    max_pitch)
    );
  }

  PLOGI << "Loaded all sound effects!";
}

tuple<float, float> AudioManager::getPitchValues(toml::value sound_data) {
  float min_pitch, max_pitch;

  max_pitch = toml::find_or(sound_data, "pitch_max", 1.0);
  min_pitch = toml::find_or(sound_data, "pitch_min", max_pitch);

  PLOGD << "Min Pitch: " << min_pitch << ", Max Pitch: " << max_pitch;
  return make_tuple(min_pitch, max_pitch);
}
