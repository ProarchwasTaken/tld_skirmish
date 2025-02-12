// cmd_heavy_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "base/action_command.h"
#include "combatants/char_player.h"


/* The HeavyAttack is a action command that's exclusive to the player
 * character, and it could be performed by pressing a button.
 * It functions the same as the Light Attack but it inflicts more damage,
 * and it can damage multiple enemies at once in exchange for longer 
 * charge and recovery time.*/
class HeavyAttack : public ActionCommand {
public:
  HeavyAttack(PlayerCharacter *user);

  void setupHurtbox();
  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;

  void enemyHitCheck();

  void drawDebug() override;

  bool attack_connected = false;
private:
  Rectangle hurtbox;
  combatant_list *enemies;
  PlayerCharacter *player;

  uint16_t damage;
  float stun_time;
  float guard_pierce;

  bool decelerate = false;
  float percentage = 1.0;
};
