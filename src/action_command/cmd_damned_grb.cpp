// action_command/cmd_damned_grb.cpp
#include <cstddef>
#include <raylib.h>
#include <random>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "enemy_damned.h"
#include "cmd_damned_grb.h"

using std::uniform_int_distribution;


DamnedGrab::DamnedGrab(DamnedEnemy *user):
  ActionCommand(user, "Damned Grab", CMD_NONE, 0.8, 0.1, 0.5) 
{
  this->player = user->player;
}

DamnedGrab::~DamnedGrab() {
  user->current_anim = NULL;
}

void DamnedGrab::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  Animation::play(this->user, sprites::damned, anim_charge, 
                  charge_frametime);

  if (finished_charge) {
    user->current_sprite = sprites::damned[8];
  }
}

void DamnedGrab::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  if (finished_action) {
    user->current_sprite = sprites::damned[9];
  }
}

void DamnedGrab::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);

  if (finished_recovering) {
    uniform_int_distribution<int> range(5, 10);
    auto *enemy = static_cast<DamnedEnemy*>(user);
    enemy->cooldown_patience = range(RNG::generator);
  }
}
