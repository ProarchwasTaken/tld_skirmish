// defaults.h
#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

constexpr float CANVAS_WIDTH = 426;
constexpr float CANVAS_HEIGHT = 240;

#define CAMERA_BOUNDS 512

#define TARGET_FRAMERATE 60

#ifdef NDEBUG
  #define DEV_BUILD 0
#else
  #define DEV_BUILD 1
#endif
