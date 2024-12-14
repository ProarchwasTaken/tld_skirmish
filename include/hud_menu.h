// hud_menu.h
#pragma once
#include <raylib.h>

#define SPADE_STANDBY 0
#define SPADE_FADEIN 1
#define SPADE_FADEOUT 2

class MenuHud {
public:
  MenuHud(bool open_sequence);
  void fadeInSpade();
  void fadeOutSpade();

  void update();
  void gradientShifting();

  void openingSequence();
  void spadeSequence();

  void drawTopBar();
  void drawBottomBar();
  void drawGradient();
  void draw();

  bool opening;

  int spade_state = SPADE_STANDBY;
  float spade_percentage;
private:
  float open_percentage = 0.0;
  float open_time = 0.25;

  float top_bar_y = 0;
  float bottom_bar_y = 206;

  Color gradient_tint = WHITE;
  float gradient_offset = 0;

  float spade_lerp_time = 0.25;

  Color spade_tint = {255, 255, 255, 0};
  float spade_x = 15;

  float shift_time = 0.1;
  float shift_timestamp;
};
