// action_command/cmd_ball_heavy.cpp
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/action_command.h"
#include "utils.h"
#include "proj_ball.h"
#include "char_player.h"
#include "cmd_ball_heavy.h"


BallHeavy::BallHeavy(PlayerCharacter *player):
  ActionCommand(player, "Ball Heavy", CMD_TECH_HEAVY, 0.5, 0.1, 0.2)
{
  enemies = player->enemies;
  sub_weapon = player->sub_weapon.get();

  user->current_sprite = sprites::player[24];
}

void BallHeavy::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->current_sprite = sprites::player[26];
  }
}

void BallHeavy::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  if (finished_action) {
    user->current_sprite = sprites::player[27];
  }
}
