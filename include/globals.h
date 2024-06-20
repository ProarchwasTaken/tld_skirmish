// globals.h
/* This file contains a number of macros and external variables that all
 * have the capacity to be different depending on the circumstances. Like
 * what command line arguments are used when the game is ran, or what
 * platform the game is running on.*/
#pragma once
#include <raylib.h>
#include <vector>
#include "audio.h"

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

namespace audio {
  extern std::vector<SoundMetaData> sfx_metadata;
}

namespace sprites {
  extern std::vector<Texture*> player;
  extern std::vector<Texture*> ghoul;
}

extern bool DEBUG_MODE;
