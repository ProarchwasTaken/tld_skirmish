// hud_life.h
#pragma once
#include <raylib.h>
#include "char_player.h"


class LifeHud {
public:
  LifeHud(PlayerCharacter *player);

  void alignText(const char* health_text);
  void draw();

private:
  PlayerCharacter *player;
  
  Vector2 hud_position;
  Vector2 text_position;
};
