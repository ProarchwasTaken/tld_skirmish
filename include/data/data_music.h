// data/data_music.h
#pragma once
#include <string>


struct MusicMetaData {
  const std::string path;
  const int id;
  const bool looping;

  float prev_time = 0;
};

struct VolumeLerpData {
  bool active = false;
  float lerp_time = 0.0;
  float end_volume = 1.0;

  float percentage = 1.0;
};
