// cmd_gun_heavy.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/generics.h"
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


class GunHeavy : public ActionCommand {
public:
  GunHeavy(PlayerCharacter *player);
  ~GunHeavy();

  void chargeSequence(float time_elapsed) override;
  bool techInputHeldDown();
private:
  combatant_list *enemies;

  PlayerCharacter *player;
  SubWeapon *sub_weapon;

  bool input_released = false;

  std::vector<int> anim_charge = {34, 33};
  float flash_interval = 0.05;

  std::vector<int> anim_shoot = {37, 38};
  float shoot_frametime = 0.10;
};
