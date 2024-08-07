// enemy_dummy.h
#pragma once
#include <raylib.h>
#include "base/combatant.h"
#include "char_player.h"


/* The practice dummy's purpose is that it's meant to be something to test
 * attacks and certain features on. It's doesn't exactly do anything 
 * special besides deriving from the combatant class, and it should never
 * be seen in the base game.*/
class DummyEnemy : public Combatant{
public:
  DummyEnemy(PlayerCharacter &player, Vector2 position);

  void update() override;
  void draw(Vector2 &camera_target) override;
private:
  PlayerCharacter *player;
};
