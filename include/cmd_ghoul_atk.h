// cmd_ghoul_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "enemy_ghoul.h"
#include "char_player.h"
#include "base/action_command.h"


class GhoulAttack : public ActionCommand {
public:
  GhoulAttack(GhoulEnemy *user);

  void setupHurtbox();
  void actSequence(float time_elapsed) override;

  void playerHitCheck();
  void drawDebug() override;
private:
  Rectangle hurtbox;
  PlayerCharacter *player;

  uint16_t damage;
  float stun_time;
};
