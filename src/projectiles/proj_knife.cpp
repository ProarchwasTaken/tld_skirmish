// projectiles/proj_knife.cpp
#include <raylib.h>
#include <cstdint>
#include "defaults.h"
#include "globals.h"
#include "base/generics.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include "proj_knife.h"
#include <plog/Log.h>


KnifeProjectile::KnifeProjectile(Vector2 position, int8_t direction,
                                 combatant_list *enemies):
  DynamicActor(position, TYPE_PROJECTILE, {8, 8}, {16, 16}, {-4, -4}) 
{
  this->enemies = enemies;
  this->direction = direction;

  damage = 4;
  stun_time = 0.2;

  velocity_x = 3.0;

  // Always remember to set the current_sprite ahead of time!
  // Learned that the hard way...
  current_sprite = sprites::weapon_knife[0];
  anim_spin = {0, 1, 2, 3, 4, 5, 6, 7};
  spin_frametime = 0.05;

}

void KnifeProjectile::update(double &delta_time) {
  Animation::play(this, sprites::weapon_knife, anim_spin, spin_frametime);
  movement(delta_time);
  enemyHitCheck();
}

void KnifeProjectile::movement(double &delta_time) {
  position.x += (velocity_x * delta_time) * direction;

  if (position.x < -CAMERA_BOUNDS || position.x > CAMERA_BOUNDS) {
    awaiting_deletion = true;
  }

  hitboxCorrection();
  texRectCorrection();
}

void KnifeProjectile::enemyHitCheck() {
  for (auto &enemy : *enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hitbox, enemy->hitbox)) {
      enemy->takeDamage(damage, 0.0, stun_time);
      awaiting_deletion = true;
      return;
    }
  }
}

void KnifeProjectile::draw() {
  Actor::draw();
  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  Rectangle dest = {tex_position.x, tex_position.y, 
    tex_scale.x, tex_scale.y};

  if (direction == -1) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, dest, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  } 
}
