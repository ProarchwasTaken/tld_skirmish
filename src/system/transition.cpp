// system/transition.cpp
#include <cassert>
#include <raylib.h>
#include <raymath.h>
#include <triblib/globals.h>
#include "defaults.h"
#include "sys_transition.h"
#include <plog/Log.h>

void WipeTransition::interpolate() {
  if (active == false) {
    return;
  }

  if (fading_out == false) {
    fade_percentage -= GetFrameTime() / fade_time;
  }
  else {
    fade_percentage += GetFrameTime() / fade_time;
  }

  fade_percentage = Clamp(fade_percentage, 0.0, 1.0);
  fade_width = Lerp(0.0, CANVAS_WIDTH / 2, fade_percentage);

  float time_elapsed = CURRENT_TIME - fade_timestamp;
  if (time_elapsed >= fade_time) {
    PLOGI << "Transition Complete.";
    active = false;
  }
}

void WipeTransition::fadein(float fade_time, Color effect_color) {
  this->fade_width = CANVAS_WIDTH / 2;
  this->fade_percentage = 1.0;

  fading_out = false;
  active = true;

  PLOGI << "Fading in...";
  updateProperties(fade_time, effect_color);
}

void WipeTransition::fadeout(float fade_time, Color effect_color) {
  this->fade_width = 0;
  this->fade_percentage = 0.0;

  fading_out = true;
  active = true;

  PLOGI << "Fading out...";
  updateProperties(fade_time, effect_color);
}

void WipeTransition::updateProperties(float fade_time, Color color) {
  PLOGD << "Fade Time: " << fade_time;
  this->effect_color = color;
  this->fade_time = fade_time;
  this->fade_timestamp = CURRENT_TIME;
}

void WipeTransition::draw() {
  if (fade_width == 0) {
    return;
  }

  Rectangle bar = {0, 0, fade_width, CANVAS_HEIGHT};
  DrawRectangleRec(bar, effect_color);

  bar.x = CANVAS_WIDTH;
  DrawRectanglePro(bar, {fade_width, 0}, 0, effect_color);
}
