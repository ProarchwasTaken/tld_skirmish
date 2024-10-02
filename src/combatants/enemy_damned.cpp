// combatants/enemy_damned.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "utils_camera.h"
#include "utils_animation.h"
#include "char_player.h"
#include "enemy_damned.h"


DamnedEnemy::DamnedEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Damned", TYPE_ENEMY, DAM_HP, DAM_STABILITY, position,
            DAM_HITBOX_SCALE, {64, 64}, DAM_HITBOX_OFFSET)
{
  this->player = &player;
  current_sprite = sprites::damned[1];

  anim_walk = {0, 1, 2, 1};
  walk_frametime = 0.5;
}

void DamnedEnemy::update() {
  switch (state) {
    case NEUTRAL: {
      Animation::play(this, sprites::damned, anim_walk, walk_frametime);
      break;
    } 
    case HIT_STUN: {
      applyKnockback(512);
      stunSequence();
      break;
    }
    case DEAD: {
      awaiting_deletion = true;
      break;
    }
    default: {
      commandSequence();
    }
  }
}

void DamnedEnemy::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  if (direction == LEFT) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  }
}

void DamnedEnemy::drawDebug() {
  Actor::drawDebug();

  if (isUsingCommand()) {
    current_command->drawDebug();
  }
}
