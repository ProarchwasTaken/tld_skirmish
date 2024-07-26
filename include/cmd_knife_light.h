// cmd_knife_light.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"

class KnifeLight : public ActionCommand {
public:
  KnifeLight(PlayerCharacter *player);

  void setupHurtbox();
  void chargeSequence(float time_elapsed, double &delta_time) override;
  void actSequence(float time_elapsed, double &delta_time) override;

  void enemyHitCheck();
  void drawDebug() override;

  bool attack_connected = false;
private:
  Rectangle hurtbox;
  combatant_list *enemies;

  uint16_t damage;
  float guard_pierce;
  float stun_time;
};
