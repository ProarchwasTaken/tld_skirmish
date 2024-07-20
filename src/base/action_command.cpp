// action_command.cpp
#include <string>
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include <plog/Log.h>

using std::string;


ActionCommand::ActionCommand(Combatant *user, string command_name,
                             float charge_time, float act_time, 
                             float recovery_time)
{
  this->command_name = command_name;
  this->user = user;
  user->state = CHARGING;

  this->charge_time = charge_time;
  this->act_time = act_time;
  this->recovery_time = recovery_time;

  PLOGD << "{Combatant: " << user->name << "} is now attempting to use "
    << "{Command: "<< command_name << "}";
  sequence_timestamp = CURRENT_TIME;
}

void ActionCommand::chargeSequence(float time_elapsed, double &delta_time) 
{
  finished_charge = time_elapsed >= charge_time;

  if (finished_charge) {
    user->state = ACT;
    sequence_timestamp = CURRENT_TIME;
  }
}

void ActionCommand::actSequence(float time_elapsed, double &delta_time) {
  finished_action = time_elapsed >= act_time;

  if (finished_action) {
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void ActionCommand::recoverySequence(float time_elapsed, 
                                     double &delta_time) 
{
  finished_recovering = time_elapsed >= recovery_time;

  if (finished_recovering) {
    user->state = NEUTRAL;
  }
}
