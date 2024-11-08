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

  void chargeAnimation();
  void charge();

  bool techInputHeldDown();
  void shootAnimation();
private:
  combatant_list *enemies;

  PlayerCharacter *player;
  SubWeapon *sub_weapon;

  bool input_released = false;

  uint8_t level = 1;
  uint8_t max_level = 3;

  float level_interval = 1.0;
  float level_timestamp;

  std::vector<int> anim_charge = {34, 33};
  std::vector<int> anim_altcharge = {36, 35};
  float charge_frametime = 0.05;

  std::vector<int> anim_shoot = {37, 38};
  std::vector<int> anim_altshoot = {39, 40};
  float shoot_frametime = 0.10;
};
