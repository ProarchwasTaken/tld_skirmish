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
  GunHeavy(PlayerCharacter *player, uint8_t start_level);
  ~GunHeavy();

  void chargeSequence(float time_elapsed) override;

  void chargeAnimation();
  void charge();
  void moraleDrain();

  bool techInputHeldDown();
  void shootAnimation();

  Rectangle setupHurtbox();
  void shoot();

  void recoverySequence(float time_elapsed) override;
private:
  combatant_list *enemies;

  PlayerCharacter *player;
  SubWeapon *sub_weapon;

  bool input_released = false;

  uint8_t level;
  uint8_t max_level = 3;

  float level_interval = 1.0;
  float level_timestamp;

  float drain_time = 0.20;
  float drain_timestamp = 0;

  uint8_t min_damage = 4;
  float min_range = 64;
  float min_stuntime = 0.5;

  std::vector<int> anim_charge = {34, 33};
  std::vector<int> anim_altcharge = {36, 35};
  float charge_frametime = 0.05;

  std::vector<int> anim_shoot = {37, 38};
  std::vector<int> anim_altshoot = {39, 40};
  float shoot_frametime = 0.10;
};
