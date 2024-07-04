// globals.h
/* This file contains a number of macros and external variables that all
 * have the capacity to be different depending on the circumstances. Like
 * what command line arguments are used when the game is ran, or what
 * platform the game is running on.*/
#pragma once
#include <raylib.h>
#include <vector>
#include <random>
#include "audio.h"
#include "sprite_loader.h"

#define WINDOWS 0
#define LINUX 1

#if defined(_WIN32)
  #define PLATFORM WINDOWS
#elif defined(__linux__)
  #define PLATFORM LINUX
#elif defined(__unix__)
  #define PLATFORM LINUX
#endif

namespace COLORS {
  extern Color *PALETTE;
}

namespace fonts {
  extern Font *skirmish;
}

namespace audio {
  extern std::vector<SoundMetaData> sfx_metadata;
}

namespace sprites {
  extern std::vector<Texture*> hud_life;
  extern std::vector<SpriteMetaData> hud_life_metadata;

  extern std::vector<Texture*> player;
  extern std::vector<SpriteMetaData> plr_metadata;

  extern std::vector<Texture*> ghoul;
  extern std::vector<SpriteMetaData> gol_metadata;
}

namespace RNG {
  extern std::mt19937 generator;
}

extern bool DEBUG_MODE;

