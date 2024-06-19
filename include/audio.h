// audio.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <string>

class SoundData {
public:
  SoundData(std::string name, uint8_t id);

  std::string name;
  uint8_t id;
};
