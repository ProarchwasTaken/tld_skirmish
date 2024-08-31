// cmd_ball_heavy.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "base/generics.h"
#include "base/sub-weapon.h"
#include "base/action_command.h"


class BallHeavy : public ActionCommand {
public:
  BallHeavy(PlayerCharacter *player);

  void chargeSequence(float time_elasped) override;
  void actSequence(float time_elasped) override;

  void enemyHitCheck();
  void createProjectile();

  bool normal_kick = false;
  bool attack_connected;

  Rectangle hurtbox;
private:
  combatant_list *enemies;
  SubWeapon *sub_weapon;

  uint16_t damage;
  float guard_pierce;
  float stun_time;
};
