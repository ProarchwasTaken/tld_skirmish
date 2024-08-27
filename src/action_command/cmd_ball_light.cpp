// action_command/cmd_ball_light.cpp
#include "globals.h"
#include "base/action_command.h"
#include "utils.h"
#include "char_player.h"
#include "cmd_ball_light.h"


BallLight::BallLight(PlayerCharacter *user):
  ActionCommand(user, "Ball Light", CMD_TECH_LIGHT, 0.3,  0.45, 0.6)
{
  anim_juke = {20, 21, 22, 21};
  juke_frametime = 0.15;

  user->current_sprite = sprites::player[19];
}

void BallLight::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->invulnerable = true;
  }
}

void BallLight::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);
  Animation::play(user, sprites::player, anim_juke, juke_frametime);

  if (finished_action) {
    user->current_sprite = sprites::player[23];
    user->invulnerable = false;
  }
}
