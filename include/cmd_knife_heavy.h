// cmd_knife_heavy.h
#pragma once
#include <raylib.h>
#include "base/generics.h"
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include "char_player.h"

class KnifeHeavy : public ActionCommand {
public:
  KnifeHeavy(PlayerCharacter *player);

  void chargeSequence(float time_elapsed, double &delta_time) override;
private:
  combatant_list *enemies;
  SubWeapon *sub_weapon;
};
