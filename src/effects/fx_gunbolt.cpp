// effects/fx_gunbolt.cpp
#include "globals.h"
#include "base/combatant.h"
#include "base/dynamic_actor.h"
#include "utils_animation.h"
#include "utils_camera.h"
#include "char_player.h"
#include <raylib.h>
#include "fx_gunbolt.h"


GunBolt::GunBolt(PlayerCharacter *player, float length): 
  DynamicActor({0, 0}, TYPE_PARTICLE_FX, {1, 1}, {16 + length, 16},
               {0, 0}, {0, -8}) 
{
  this->player = player;
  this->current_sprite = sprites::weapon_gun[1];

  position.x = player->position.x + (29 * player->direction);
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
