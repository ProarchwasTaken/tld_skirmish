// cmd_light_atk.h
#pragma once
#include "base/combatant.h"
#include "base/action_command.h"


class LightAttack : public ActionCommand {
public:
  LightAttack(Combatant &user);

  void draw();
  void drawDebug();
};
