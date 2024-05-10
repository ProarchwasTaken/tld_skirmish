// action_command.h
#pragma once
#include <string>
#include "generics.h"


class ActionCommand {
public:
  ActionCommand(Combatant &user, std::string command_name, 
                float charge_time, float act_time, float recovery_time);

  virtual void chargeSequence(float time_elapsed);
  virtual void actSequence(float time_elapsed);
  virtual void recoverySequence(float time_elapsed);

  float sequence_timestamp;
private:
  std::string command_name;
  Combatant *user;

  float charge_time;
  float act_time;
  float recovery_time;
};
