// cmd_heavy_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "char_player.h"
#include "base/action_command.h"


class HeavyAttack : public ActionCommand {
public:
  HeavyAttack(PlayerCharacter *player);

  void setupHurtbox();
  void actSequence(float time_elapsed) override;

  void enemyHitCheck();

  void draw() override;
  void drawDebug() override;
private:
  Rectangle hurtbox;

  uint16_t damage;
  float stun_time;
};
