// globals
#pragma once
#include <raylib.h>

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

extern bool DEBUG_MODE;
