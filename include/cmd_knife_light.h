// cmd_knife_light.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"

/* Light Technique that's associated with the Knife SubWeapon. Upon use,
 * the player will perform a simple slash attack.*/
class KnifeLight : public ActionCommand {
public:
  KnifeLight(PlayerCharacter *player);

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
};
