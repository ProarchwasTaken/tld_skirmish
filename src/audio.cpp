// audio.h
#include <cstdint>
#include <raylib.h>
#include <string>
#include <toml/parser.hpp>
#include <toml/value.hpp>
#include "globals.h"
#include "audio.h"
#include <plog/Log.h>

using std::string;


SoundData::SoundData(string name, uint8_t id, AudioManager *manager) {
  this->name = name;
  this->id = id;

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

    Sound sound = LoadSound(sound_path.c_str());
    sound_effects.push_back(sound);

    audio::sfx_metadata.push_back(SoundData(sound_name, index, this));
  }

  PLOGI << "Loaded all sound effects!";
}
