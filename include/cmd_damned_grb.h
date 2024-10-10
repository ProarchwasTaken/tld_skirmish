// cmd_damned_grb.h
#pragma once
#include <raylib.h>
#include <vector>
#include "enemy_damned.h"
#include "char_player.h"
#include "base/action_command.h"


class DamnedGrab : public ActionCommand {
public:
  DamnedGrab(DamnedEnemy *user);
  ~DamnedGrab();

  void setupHurtbox();

  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;
  void recoverySequence(float time_elapsed) override;
private:
  PlayerCharacter *player;

  std::vector<int> anim_charge = {6, 7};
  float charge_frametime = 0.4;
};
