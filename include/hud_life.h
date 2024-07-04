// hud_life.h
#pragma once
#include <raylib.h>
#include "char_player.h"


class LifeHud {
public:
  LifeHud(PlayerCharacter *player);

  void draw();

private:
  PlayerCharacter *player;
  
  Vector2 hud_position;
};
