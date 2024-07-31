// action_command/cmd_knife_heavy.h
#include <raylib.h>
#include "globals.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_knife_heavy.h"


KnifeHeavy::KnifeHeavy(PlayerCharacter *player):
  ActionCommand(player, "Knife Heavy", CMD_TECH_HEAVY, 0.25, 0.1, 0.0) 
{
  enemies = player->enemies;
  sub_weapon = player->sub_weapon.get();

  user->current_sprite = sprites::player[13];
}

void KnifeHeavy::chargeSequence(float time_elapsed, double &delta_time) {
  ActionCommand::chargeSequence(time_elapsed, delta_time);

  if (finished_charge) {
    user->current_sprite = sprites::player[14];
  }
}
