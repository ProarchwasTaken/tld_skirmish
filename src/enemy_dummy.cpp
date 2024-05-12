// enemy_dummy.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "char_player.h"
#include "enemy_dummy.h"


DummyEnemy::DummyEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Dummy", TYPE_ENEMY, 9999, position)
{
  this->player = &player;
}

void DummyEnemy::update(double &delta_time) {
  if (state == HIT_STUN) {
    stunSequence();
  }
}

void DummyEnemy::draw() {
  if (state == NEUTRAL) {
    DrawRectangleRec(hitbox, MAGENTA);
  }
  else {
    DrawRectangleRec(hitbox, ORANGE);
  }

  if (DEBUG_MODE) {
    drawDebug();
  }
}
