// hud/hud_menu.cpp
#include <cassert>
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "hud_menu.h"

constexpr float START_TOP_Y = -34;
constexpr float START_BOTTOM_Y = 240;

constexpr float START_SPADE_X = -15;
constexpr float DEF_SPADE_X = 15;


MenuHud::MenuHud(bool open_sequence) {
  opening = open_sequence;
  if (opening) {
    top_bar_y = START_TOP_Y;
    bottom_bar_y = START_BOTTOM_Y;
    gradient_tint.a = 0;
  }

  shift_timestamp = CURRENT_TIME;
}

void MenuHud::fadeInSpade() {
  spade_x = START_SPADE_X;
  spade_tint.a = 0;
  spade_percentage = 0.0;

  spade_state = SPADE_FADEIN;
}

void MenuHud::fadeOutSpade() {
  spade_x = DEF_SPADE_X;
  spade_tint.a = 255;
  spade_percentage = 1.0;

  spade_state = SPADE_FADEOUT;
}

void MenuHud::update() {
  gradientShifting();

  if (opening) {
    openingSequence();
  }
  else if (spade_state != SPADE_STANDBY) {
    spadeSequence(); 
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

void MenuHud::spadeSequence() {
  assert(spade_state != SPADE_STANDBY);

  switch (spade_state) {
    case SPADE_FADEIN: {
      spade_percentage += GetFrameTime() / spade_lerp_time;
      break;
    }
    case SPADE_FADEOUT: {
      spade_percentage -= GetFrameTime() / spade_lerp_time;
      break;
    }
  }

  spade_percentage = Clamp(spade_percentage, 0.0, 1.0);
  spade_x = Lerp(START_SPADE_X, DEF_SPADE_X, spade_percentage);
  spade_tint.a = Lerp(0, 255, spade_percentage);

  bool finished = spade_percentage == 0.0 || spade_percentage == 1.0;
  if (finished) {
    spade_state = SPADE_STANDBY;
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

  DrawTexture(*sprites::hud_mainmenu[4], spade_x, 34, spade_tint);
}
