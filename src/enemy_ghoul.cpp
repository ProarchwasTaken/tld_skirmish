// enemy_ghoul.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "char_player.h"
#include "enemy_ghoul.h"


GhoulEnemy::GhoulEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Ghoul", TYPE_ENEMY, GOL_HP, position, GOL_HITBOX_SCALE,
            {64, 64}, GOL_HITBOX_OFFSET) 
{
  this->player = &player;
}

void GhoulEnemy::update(double &delta_time) {
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

void GhoulEnemy::draw() {
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
