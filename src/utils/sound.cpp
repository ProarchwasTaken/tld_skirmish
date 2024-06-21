// utils/sound.cpp
#include <cstddef>
#include <cstdint>
#include <raylib.h>
#include "globals.h"
#include "audio.h"
#include "utils.h"
#include <plog/Log.h>


void SoundUtils::play(std::string sound_name) {
  SoundMetaData *meta_data = getMetaData(sound_name);

  if (meta_data == NULL) {
    PLOGE << "Unable to find meta data for sound: " << sound_name << "!";
    return;
  }

  uint8_t sound_id = meta_data->id;
  Sound *sound = &meta_data->manager->sound_effects[sound_id];

  PlaySound(*sound);
}


SoundMetaData *SoundUtils::getMetaData(std::string &sound_name) {
  SoundMetaData *meta_data = NULL;

  for (SoundMetaData data : audio::sfx_metadata) {
    if (sound_name == data.name) {
      meta_data = &data;
    }
  }

  return meta_data;
}
