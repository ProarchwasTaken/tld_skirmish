// projectiles/proj_knife.cpp
#include <raylib.h>
#include <cstdint>
#include "globals.h"
#include "base/generics.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include "proj_knife.h"


KnifeProjectile::KnifeProjectile(Vector2 position, int8_t direction,
                                 combatant_list *enemies):
  DynamicActor(position, TYPE_PROJECTILE) 
{
  this->enemies = enemies;
  this->direction = direction;

  damage = 4;

  stun_time = 0.2;

  anim_spin = {0, 1, 2, 3, 4, 5, 6, 7};
  spin_frametime = 0.1;
}

void KnifeProjectile::update(double &delta_time) {
  Animation::play(this, sprites::weapon_knife, anim_spin, spin_frametime);
}

void KnifeProjectile::draw() {
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
