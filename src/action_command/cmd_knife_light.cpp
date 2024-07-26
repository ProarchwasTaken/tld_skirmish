// action_command/cmd_knife_light.cpp
#include <raylib.h>
#include "globals.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_knife_light.h"


KnifeLight::KnifeLight(PlayerCharacter *user):
ActionCommand(user, "Knife Light", 0.2, 0.1, 0.3)
{
  damage = 3;
  guard_pierce = 3.0;

  stun_time = 0.5;

  this->enemies = user->enemies;
  user->current_sprite = sprites::player[10]; 
}

void KnifeLight::chargeSequence(float time_elapsed, double &delta_time) {
  ActionCommand::chargeSequence(time_elapsed, delta_time);

  if (finished_charge) {
    user->current_sprite = sprites::player[11];
  }
}

void KnifeLight::actSequence(float time_elapsed, double &delta_time) {
  ActionCommand::actSequence(time_elapsed, delta_time);

  if (finished_action) {
    user->current_sprite = sprites::player[12];
  }
}
