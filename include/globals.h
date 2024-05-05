// color_palette.h
#pragma once
#include <raylib.h>

#if defined(_WIN32)
  #define PLATFORM "windows"
#elif defined(__linux__)
  #define PLATFORM "linux"
#elif defined(__unix__)
  #define PLATFORM "linux"
#endif

namespace COLORS {
  extern Color *PALETTE;
}

extern bool DEBUG_MODE;
