// cmd_light_atk.h
#pragma once
#include <raylib.h>
#include "base/combatant.h"
#include "base/action_command.h"


class LightAttack : public ActionCommand {
public:
  LightAttack(Combatant &user);

  void setupHurtbox();

  void draw();
  void drawDebug();
private:
  Rectangle hurtbox;
};
