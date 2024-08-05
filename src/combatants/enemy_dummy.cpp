// combatants/enemy_dummy.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "char_player.h"
#include "enemy_dummy.h"


DummyEnemy::DummyEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Dummy", TYPE_ENEMY, 9999, 1.0,position)
{
  this->player = &player;
}

void DummyEnemy::update() {
  switch (state) {
    case HIT_STUN: {
      stunSequence();
      break;
    }
    case DEAD: {
      awaiting_deletion = true;
    }
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
