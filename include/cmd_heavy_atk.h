// cmd_heavy_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "char_player.h"
#include "base/action_command.h"


/* The HeavyAttack is a action command that's exclusive to the player
 * character, and it could be performed by pressing a button.
 * It functions the same as the Light Attack but it inflicts more damage
 * in exchange for longer charge and recovery time. If the player hits
 * a light attack, this ActionCommand could be performed immediantly, 
 * skipping the recovery time the light attack usually provides.*/
class HeavyAttack : public ActionCommand {
public:
  HeavyAttack(PlayerCharacter *player);

  void setupHurtbox();
  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;

  void enemyHitCheck();

  void draw() override;
  void drawDebug() override;

  bool attack_connected = false;
private:
  Rectangle hurtbox;

  uint16_t damage;
  float stun_time;
};
