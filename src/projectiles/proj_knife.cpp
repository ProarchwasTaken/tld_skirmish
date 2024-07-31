// projectiles/proj_knife.cpp
#include <raylib.h>
#include "globals.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include "proj_knife.h"


KnifeProjectile::KnifeProjectile(Vector2 position):
  DynamicActor(position, TYPE_PROJECTILE) 
{
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

  DrawTexturePro(*current_sprite, source, dest, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  } 
}
