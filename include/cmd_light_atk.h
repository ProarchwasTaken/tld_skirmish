// cmd_light_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "base/combatant.h"
#include "char_player.h"
#include "base/action_command.h"


class LightAttack : public ActionCommand {
public:
  LightAttack(Combatant &user);

  void setupHurtbox();
  void actSequence(float time_elapsed) override;

  void enemyHitCheck(PlayerCharacter &player);

  void draw() override;
  void drawDebug() override;
private:
  Rectangle hurtbox;

  uint16_t damage;
  float stun_time;
};
