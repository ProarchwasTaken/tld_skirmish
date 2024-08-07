// cmd_light_atk.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "base/generics.h"
#include "char_player.h"
#include "base/action_command.h"


/* The light attack is an action command exclusive to the player 
 * character. Inflicts an average amount of damage, and has a low charge
 * and recovery time. The stun time would be so short that the enemy 
 * would be out of hit stun by the time the player recovers from the using
 * the attack.
 *
 * When the player hits a light attack, they could
 * immediantly cancel the attack into a heavy attack. Thus keeping the 
 * enemy in hit stun for a little bit longer.*/
class LightAttack : public ActionCommand {
public:
  LightAttack(PlayerCharacter *user);

  /* For defining the hurtbox and positioning it just in front of the
   * the player. Using called on initialization*/
  void setupHurtbox();
  void chargeSequence(float time_elapsed, double &delta_time) override;
  void actSequence(float time_elapsed, double &delta_time) override;

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
};
