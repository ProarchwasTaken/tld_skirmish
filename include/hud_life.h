// hud_life.h
#pragma once
#include <raylib.h>
#include "char_player.h"


class LifeHud {
public:
  LifeHud(PlayerCharacter *player);
  void update();

  void segmentBlinkInterval();
  void updateGauge();
  void alignText(const char* health_text);
  void draw();

private:
  PlayerCharacter *player;
  
  Vector2 hud_position;
  Vector2 text_position;

  Vector2 gauge_position;
  Rectangle gauge_source; 

  float gauge_width;
  float segment_width;

  Rectangle segment_source;
  Vector2 segment_position;
  bool display_segment = false;

  float blink_interval;
  float blink_timestamp = 0;
};
