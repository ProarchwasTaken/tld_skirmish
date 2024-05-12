// enemy_dummy.h
#pragma once
#include <raylib.h>
#include "base/combatant.h"
#include "char_player.h"

class DummyEnemy : public Combatant{
public:
  DummyEnemy(PlayerCharacter &player, Vector2 position);

  void update(double &delta_time);
  void draw();
private:
  PlayerCharacter *player;
};
