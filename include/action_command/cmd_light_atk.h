// cmd_light_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "char_player.h"
#include "base/action_command.h"


/* The light attack is an action command exclusive to the player 
 * character. Inflicts an average amount of damage, and has a low charge
 * and recovery time. The stun time would be so short that the enemy 
 * would be out of hit stun by the time the player recovers from the using
 * the attack.
 *
 * A unique property of the LightAttack is that it could be canceled into
 * any ActionCommand except itself it the attack hits. Although that sort
 * of logic is handled by the PlayerCharacter rather than the class 
 * itself.*/
class LightAttack : public ActionCommand {
public:
  LightAttack(PlayerCharacter *user);

  /* For defining the hurtbox and positioning it just in front of the
   * the player. Using called on initialization*/
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
  float guard_pierce;
  float stun_time;

  bool decelerate = false;
  float percentage = 1.0;
};
