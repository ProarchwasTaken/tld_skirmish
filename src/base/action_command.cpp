// action_command.cpp
#include <raylib.h>
#include "base/combatant.h"
#include "base/action_command.h"


ActionCommand::ActionCommand(Combatant &user, float charge_time, 
                             float act_time, float recovery_time)
{
  this->user = &user;
  user.state = CHARGING;

  this->charge_time = charge_time;
  this->act_time = act_time;
  this->recovery_time = recovery_time;

  sequence_timestamp = GetTime();
}

void ActionCommand::chargeSequence(float time_elapsed) {
  bool finished_charge = time_elapsed >= charge_time;

  if (finished_charge) {
    user->state = ACT;
    sequence_timestamp = GetTime();
  }
}

void ActionCommand::actSequence(float time_elapsed) {
  bool finished_action = time_elapsed >= act_time;

  if (finished_action) {
    user->state = RECOVER;
    sequence_timestamp = GetTime();
  }
}

void ActionCommand::recoverySequence(float time_elapsed) {
  bool finished_recovering = time_elapsed >= recovery_time;

  if (finished_recovering) {
    user->state = NEUTRAL;
  }
}
