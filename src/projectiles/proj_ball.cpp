// projectiles/proj_ball.cpp
#include <raylib.h>
#include <cstdint>
#include "globals.h"
#include "base/generics.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include "char_player.h"
#include "proj_ball.h"


BallProjectile::BallProjectile(Vector2 position, int8_t direction,
                               PlayerCharacter *player, 
                               combatant_list *enemies):
  DynamicActor(position, TYPE_PROJECTILE, {9, 9}, {13, 13}, {-4, -4.5}, 
               {-6, -6})
{
  this->player = player;
  this->enemies = enemies;
  this->direction = direction;

  anim_spin = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  spin_frametime = 0.025;

  current_sprite = sprites::weapon_ball[0];
}

void BallProjectile::update() {
  Animation::play(this, sprites::weapon_ball, anim_spin, spin_frametime);
}

void BallProjectile::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  if (direction == -1) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  } 
}
