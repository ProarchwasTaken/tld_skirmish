// utils/sound.cpp
#include <cstddef>
#include <random>
#include <raylib.h>
#include "globals.h"
#include "data/data_sound.h"
#include "utils_sound.h"
#include <plog/Log.h>

using std::uniform_real_distribution, std::string;


void SoundUtils::play(string sound_name) {
  SoundMetaData *meta_data = getMetaData(sound_name);

  if (meta_data == NULL) {
    PLOGE << "Unable to find meta data for sound: " << sound_name << "!";
    return;
  }

  Sound *sound = meta_data->sound;
  float pitch = 1.0;

  if (meta_data->random_pitch) {
    float min_pitch = meta_data->min_pitch;
    float max_pitch = meta_data->max_pitch;

    uniform_real_distribution<float> pitch_range(min_pitch, max_pitch);
    pitch = pitch_range(RNG::generator);
  }

  SetSoundPitch(*sound, pitch);
  PlaySound(*sound);
}

void SoundUtils::play(std::string sound_name, float pitch) {
  SoundMetaData *meta_data = getMetaData(sound_name);

  if (meta_data == NULL) {
    PLOGE << "Unable to find meta data for sound: " << sound_name << "!";
    return;
  }

  Sound *sound = meta_data->sound;
  SetSoundPitch(*sound, pitch);
  PlaySound(*sound);
}

void SoundUtils::stop(std::string sound_name) {
  SoundMetaData *meta_data = getMetaData(sound_name);

  if (meta_data == NULL) {
    PLOGE << "Unable to find meta data for sound: " << sound_name << "!"; 
  }

  Sound *sound = meta_data->sound;
  if (IsSoundPlaying(*sound)) {
    StopSound(*sound);
  }
  else {
    PLOGW << "Sound: " << sound_name << " is not playing!";
  }
}

void SoundUtils::pause() {
  PLOGI << "Pausing all sounds...";
  int pause_count = 0;

  for (SoundMetaData data : audio::sfx_metadata) {
    Sound *sound = data.sound;

    if (IsSoundPlaying(*sound)) {
      PauseSound(*sound);
      pause_count++;
    }
  }

  PLOGD << "Paused: " << pause_count << " sounds.";
}

void SoundUtils::resume() {
  PLOGI << "Resuming all sounds...";

  for (SoundMetaData data : audio::sfx_metadata) {
    Sound *sound = data.sound;
    ResumeSound(*sound);
  }
}

SoundMetaData *SoundUtils::getMetaData(string &sound_name) {
  SoundMetaData *meta_data = NULL;

  for (SoundMetaData data : audio::sfx_metadata) {
    if (sound_name == data.name) {
      meta_data = &data;
      break;
    }
  }

  return meta_data;
}
