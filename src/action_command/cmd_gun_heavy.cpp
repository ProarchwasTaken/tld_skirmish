// action_command/cmd_gun_heavy.cpp
#include <cstddef>
#include "globals.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "char_player.h"
#include "cmd_gun_heavy.h"

GunHeavy::GunHeavy(PlayerCharacter *player): 
  ActionCommand(player, "Gun Heavy", CMD_TECH_HEAVY, 0.4, 0.0, 1.0)
{
  this->player = player;
  this->sub_weapon = player->sub_weapon.get();

  this->enemies = player->enemies;

  player->current_sprite = sprites::player[33];
  player->frame_timestamp = CURRENT_TIME;
}

GunHeavy::~GunHeavy() {
  player->current_anim = NULL;
}

void GunHeavy::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  Animation::play(player, sprites::player, anim_shoot, shoot_frametime,
                  false);

  if (finished_charge) {
    player->current_sprite = sprites::player[41];
  }
}
