// hud_menu.h
#pragma once
#include <raylib.h>


class MenuHud {
public:
  MenuHud(bool open_sequence);

  void update();
  void gradientShifting();
  void openingSequence();

  void drawTopBar();
  void drawBottomBar();
  void drawGradient();
  void draw();

  bool opening;
private:
  float open_percentage = 0.0;
  float open_time = 0.25;

  float top_bar_y = 0;
  float bottom_bar_y = 206;

  Color gradient_tint = WHITE;
  float gradient_offset = 0;

  float shift_time = 0.1;
  float shift_timestamp;
};
