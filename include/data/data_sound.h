// data/data_sound.h
#pragma once
#include <raylib.h>
#include <string>


/* Sound meta data is meant to store infomation about a specific sound
 * effect load into the game, and make it easier for it to retrieve.
 * This kind of stuff is typically meant to be used by utility 
 * functions.*/
struct SoundMetaData {
  std::string name;
  Sound *sound;

  bool random_pitch;
  float min_pitch;
  float max_pitch;
};
