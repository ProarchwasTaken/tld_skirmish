// action_command.h
#include "combatant.h"


class ActionCommand {
public:
  ActionCommand(Combatant &user, float charge_time, float act_time,
                float recovery_time);
  virtual void chargeSequence(float time_elapsed);
  virtual void actSequence(float time_elapsed);
  virtual void recoverySequence(float time_elapsed);
private:
  Combatant *user;

  float charge_time;
  float act_time;
  float recovery_time;

  float sequence_timestamp;
};
