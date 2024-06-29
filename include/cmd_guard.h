// cmd_guard.h
#pragma once
#include <raylib.h>
#include <vector>
#include "base/combatant.h"
#include "base/action_command.h"
#include "sprite_loader.h"


class Guard : public ActionCommand {
public:
  Guard(Combatant *user, std::vector<SpriteMetaData> &data_list,
        bool can_parry = false);

  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;
  void recoverySequence(float time_elapsed) override;

private:
  bool can_parry;

  Texture* charge_sprite;
  Texture* guard_sprite;
  Texture* parry_sprite;
};
