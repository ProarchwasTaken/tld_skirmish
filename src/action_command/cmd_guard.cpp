// action_command/cmd_guard.cpp
#include <raylib.h>
#include <vector>
#include "base/combatant.h"
#include "base/action_command.h"
#include "sprite_loader.h"
#include "utils.h"
#include "cmd_guard.h"

using std::vector;


Guard::Guard(Combatant *user, vector<SpriteMetaData> &data_list,
             bool can_parry):
  ActionCommand(user, "Guard", 0.25, 0.5, 0.25) 
{
  charge_sprite = Sprite::getSprite("charge", data_list);
  guard_sprite = Sprite::getSprite("guard", data_list);

  this->can_parry = can_parry;
  if (can_parry) {
    parry_sprite = Sprite::getSprite("parry", data_list);
  }

  user->current_sprite = charge_sprite;
}

void Guard::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->current_sprite = guard_sprite;
  }
}

void Guard::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  if (finished_action) {
    user->current_sprite = charge_sprite;
  }
}

void Guard::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);
}
