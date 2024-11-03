// effects/fx_gunprobe.cpp
#include <cassert>
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/dynamic_actor.h"
#include "utils_camera.h"
#include "char_player.h"
#include "fx_gunprobe.h"


GunProbe::GunProbe(PlayerCharacter *player, Combatant *probed_enemy): 
  DynamicActor({0, 0}, TYPE_PARTICLE_FX, {8, 5}, {8, 5}, {-4, -3}, 
               {-4, -3})
{
  this->player = player;

  this->probed_enemy = probed_enemy;
  assert(probed_enemy != NULL);

  this->current_sprite = sprites::weapon_gun[0];
  positionCorrection();
}

void GunProbe::update() {
  if (shouldDelete()) {
    awaiting_deletion = true;
    return;
  }

  positionCorrection();
}

bool GunProbe::shouldDelete() {
  if (player->state == RECOVER || player->current_command == nullptr) {
    return true;
  }
  else if (player->current_command->type != CMD_TECH_LIGHT) {
    return true;
  }
  else {
    return false;
  }
}

void GunProbe::positionCorrection() {
  float x_offset = probed_enemy->position.x - player->position.x;

  if (x_offset <= 0) {
    direction = LEFT;
  }
  else {
    direction = RIGHT;
  }

  position.x = probed_enemy->position.x;
  position.x -= (tex_scale.x / 2) * direction;

  position.y = y_level;
  texRectCorrection();
  hitboxCorrection();
}

void GunProbe::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  float offset = 29 * player->direction;
  Vector2 end_position = {player->position.x + offset, y_level};
  DrawLineV(position, end_position, BLACK);

  Rectangle source = {0, 0, tex_scale.x * direction, tex_scale.y};
  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  }
}
