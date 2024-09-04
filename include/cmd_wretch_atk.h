// cmd_wretch_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "enemy_wretch.h"
#include "char_player.h"
#include "base/action_command.h"


class WretchAttack : public ActionCommand {
public:
  WretchAttack(WretchEnemy *user);

  void setupHurtbox();
  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;

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
