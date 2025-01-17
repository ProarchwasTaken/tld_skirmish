// effects/fx_gunbolt.cpp
#include <raylib.h>
#include <triblib/base/dynamic_actor.h>
#include "globals.h"
#include "base/combatant.h"
#include <triblib/utils/animation.h>
#include "utils/camera.h"
#include "combatants/char_player.h"
#include "effects/fx_gunbolt.h"


GunBolt::GunBolt(PlayerCharacter *player, float length): 
  DynamicActor({0, 0}, TYPE_PARTICLE_FX, {1, 1}, {length, 16},
               {0, 0}, {0, -8}) 
{
  this->player = player;
  this->current_sprite = sprites::weapon_gun[1];

  position.x = player->position.x + (30 * player->direction);
  position.y = player->position.y - 51;

  if (player->direction == LEFT) {
    tex_offset.x = -tex_scale.x;
  }
  texRectCorrection();
}

void GunBolt::update() {
  if (player->state != RECOVER || player->current_command == nullptr) {
    awaiting_deletion = true;
    return;
  }

  Animation::play(this, sprites::weapon_gun, anim_bolt, bolt_frametime);
}

void GunBolt::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  } 

  Rectangle source = {0, 0, tex_scale.x * player->direction, tex_scale.y};
  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  }
}
