// hud_morale.h
#pragma once
#include <raylib.h>
#include "char_player.h"


class MoraleHud {
public:
  MoraleHud(PlayerCharacter &player);
  void update();

  void updateGauge();
  void segmentBlinkInterval();

  void drawMoraleText();
  void draw();
private:
  PlayerCharacter *player;
  float morale_percentage;

  Vector2 hud_position;
  Vector2 txt_position;
};
