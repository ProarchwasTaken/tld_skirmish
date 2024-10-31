// action_command/cmd_gun_light.cpp
#include <raylib.h>
#include "globals.h"
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"
#include "utils_animation.h"
#include "cmd_gun_light.h"


GunLight::GunLight(PlayerCharacter *player): 
  ActionCommand(player, "Gun Light", CMD_TECH_LIGHT, 0.3, 0.2, 0.25) 
{
  this->player = player;
  this->enemies = player->enemies;

  player->current_sprite = sprites::player[28];
}

void GunLight::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  Animation::play(player, sprites::player, anim_charge, charge_frametime);

  if (finished_charge) {
    player->current_sprite = sprites::player[29];
  }
}

void GunLight::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  if (finished_charge) {
    player->current_sprite = sprites::player[32];
  }
}
