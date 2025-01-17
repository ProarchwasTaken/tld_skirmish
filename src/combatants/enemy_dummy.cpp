// combatants/enemy_dummy.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "utils/camera.h"
#include "combatants/char_player.h"
#include "combatants/enemy_dummy.h"


DummyEnemy::DummyEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Dummy", TYPE_ENEMY, 9999, 1.0, position, {16, 56}, {64, 64},
            {-8, -58})
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

void DummyEnemy::draw(Vector2 &camera_target) {
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

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
