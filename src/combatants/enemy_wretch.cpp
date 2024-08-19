// combatants/enemy_wretch.cpp
#include <raylib.h>
#include "globals.h"
#include "utils.h"
#include "base/combatant.h"
#include "char_player.h"
#include "enemy_wretch.h"


WretchEnemy::WretchEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Wretch", TYPE_ENEMY, WRH_HP, WRH_STABILITY, position,
            WRH_HIT_SCALE, WRH_TEX_SCALE, WRH_HIT_OFFSET, WRH_TEX_OFFSET)
{
  this->player = &player;
  current_sprite = sprites::wretch[0];

  anim_idle = {0, 1, 2, 1};
  idle_frametime = 0.4;

  anim_retreat = {3, 4, 5, 4};
  retreat_frametime = 0.4;

  anim_death = {7, 8};
  death_frametime = 0.5;
}

void WretchEnemy::update() {

}

void WretchEnemy::draw(Vector2 &camera_target) {
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

