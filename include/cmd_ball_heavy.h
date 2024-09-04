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

  void setupHurtbox();
  void chargeSequence(float time_elasped) override;
  void actSequence(float time_elasped) override;

  void enemyHitCheck();
  void createProjectile();

  void drawDebug() override;

  bool normal_proc;
  bool attack_connected = false;

  Rectangle hurtbox;
private:
  combatant_list *enemies;
  SubWeapon *sub_weapon;

  uint16_t damage;
  float guard_pierce;
  float stun_time;
};
