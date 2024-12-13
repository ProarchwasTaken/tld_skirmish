// hud/hud_menu.cpp
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "hud_menu.h"

constexpr float START_TOP_Y = -34;
constexpr float START_BOTTOM_Y = 240;


MenuHud::MenuHud(bool open_sequence) {
  opening = open_sequence;
  if (opening) {
    top_bar_y = START_TOP_Y;
    bottom_bar_y = START_BOTTOM_Y;
    gradient_tint.a = 0;
  }

  shift_timestamp = CURRENT_TIME;
}

void MenuHud::update() {
  gradientShifting();

  if (opening) {
    openingSequence();
  }
}

void MenuHud::gradientShifting() {
  float time_elapsed = CURRENT_TIME - shift_timestamp;
  if (time_elapsed >= shift_time) {
    gradient_offset++;
    shift_timestamp = CURRENT_TIME;
  }
  else {
    return;
  }

  if (gradient_offset >= 4) {
    gradient_offset = 0;
  }
}

void MenuHud::openingSequence() {
  open_percentage += GetFrameTime() / open_time;
  open_percentage = Clamp(open_percentage, 0.0, 1.0);

  top_bar_y = Lerp(START_TOP_Y, 0, open_percentage);
  bottom_bar_y = Lerp(206, START_BOTTOM_Y, 1.0 - open_percentage);
  gradient_tint.a = Lerp(0, 255, open_percentage);

  if (open_percentage == 1.0) {
    opening = false;
  }
}

void MenuHud::drawTopBar() {
  Rectangle source = {117, 0, 8, 34};
  Rectangle dest = {128, top_bar_y, 298, 34};

  DrawTexture(*sprites::hud_mainmenu[5], 0, top_bar_y, WHITE);
  DrawTexturePro(*sprites::hud_mainmenu[5], source, dest, {0, 0}, 0, 
                 WHITE);
}

void MenuHud::drawBottomBar() {
  Rectangle source = {0, 0, 32, 34};
  Rectangle dest = {0, bottom_bar_y, 852, 34};
  DrawTexturePro(*sprites::hud_mainmenu[6], source, dest, {0, 0}, 0, 
                 WHITE);
}

void MenuHud::drawGradient() {
  Rectangle source = {gradient_offset, 0, 426, 172};
  Rectangle dest = {0, 34, 426, 172};
  DrawTexturePro(*sprites::hud_mainmenu[3], source, dest, {0, 0}, 0, 
                 gradient_tint);
}

void MenuHud::draw() {
  drawTopBar();
  drawBottomBar();
  drawGradient();
}
