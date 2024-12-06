// data/data_music.h
#pragma once
#include <string>


struct MusicMetaData {
  const std::string path;
  const int id;
  const bool looping;

  float prev_time = 0;
};
