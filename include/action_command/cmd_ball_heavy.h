// cmd_ball_heavy.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "base/sub-weapon.h"
#include "base/action_command.h"


/* Heavy weapon technique that's associated with the Ball Sub-Weapon.
 * Whether or not the Sub-Weapon is usable at initialization is the main
 * determining factor of this ActionCommand's functionality. During
 * "normal procedures" a BallProjectile is created, and flys in the 
 * direction the player was facing. Otherwise, the player will just 
 * perform a simple kick which has it's own uses.*/
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
