// sys_transition.h
#pragma once
#include <raylib.h>


/* System class for drawing the wipe transition over the screen.*/
class WipeTransition {
public:
  void interpolate();

  void fadein(float fade_time, Color effect_color);
  void fadeout(float fade_time, Color effect_color);

  void updateProperties(float fade_time, Color effect_color);

  void draw();

  bool active = false;
private:
  bool fading_out = false;
  Color effect_color = BLACK;

  float fade_width = 0;
  float fade_percentage = 0;

  float fade_time = 0.0;
  float fade_timestamp = 0.0;
};
