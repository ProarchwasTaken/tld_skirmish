// cmd_ghoul_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "enemy_ghoul.h"
#include "char_player.h"
#include "base/action_command.h"


/* ActionCommand exclusive to the Ghoul enemy. Deals a moderate amount of 
 * damage, knockback, hit stun.*/
class GhoulAttack : public ActionCommand {
public:
  GhoulAttack(GhoulEnemy *user);

  void setupHurtbox();
  void chargeSequence(float time_elapsed, double &delta_time) override;
  void actSequence(float time_elapsed, double &delta_time) override;

  void playerHitCheck();
  void drawDebug() override;
private:
  Rectangle hurtbox;
  PlayerCharacter *player;

  uint16_t damage;
  float stun_time;
  float guard_pierce;

  bool attack_connected = false;
};
