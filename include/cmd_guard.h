// cmd_guard.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <vector>
#include "base/combatant.h"
#include "base/action_command.h"
#include "sprite_loader.h"

#define DEF_PARRY_WINDOW 0.20


/* Guard is an Action Command designed in a way that every combatant can
 * use it. Guarding is mean to act as a reliable defensive option for the 
 * player, and to provide some amount of mechanical variety for the 
 * enemies. Given the nature of the game, it should be important to go 
 * with the assumption that this action command will be used most often
 * by the  PlayerCharacter than other combatants.*/
class Guard : public ActionCommand {
public:
  Guard(Combatant *user, std::vector<SpriteMetaData> &data_list,
        uint16_t boundary, bool can_parry = false);

  void chargeSequence(float time_elapsed, double &delta_time) override;
  void actSequence(float time_elapsed, double &delta_time) override;
  void recoverySequence(float time_elapsed, double &delta_time) override;

  /* This method is automatically called when the user takes damage while
   * they are using this action command. Contains the majority of the 
   * logic for guarding and parrying, and it's practically the root
   * function.*/
  void guardLogic(uint16_t &dmg_magnitude, float guard_pierce, 
                  float stun_time, float kb_velocity, 
                  uint8_t kb_direction);

  /* Every combatant has a variable called "guard_stability". It's
   * primarily used to check if the user's guard is successful or not.*/
  bool guardFailed(float guard_pierce, float stun_time, float kb_velocity,
                   uint8_t kb_direction);

  /* For applying the usual bonuses from successfully performing a guard
   * or parrying an attack. These bonuses would be automatically reverted
   * once the recovery sequence ends. If it wasn't canceled for any 
   * reason.*/
  void applyGuardBonus(float stun_time, float kb_velocity = 0, 
                       uint8_t kb_direction = 0);

  /* Returns true if the user has parried the attack. How the method knows
   * this is by checking if the user was hit within a certain timeframe.*/
  bool parriedAttack(float guard_pierce, float stun_time);

  /* When the user's guard is successful, they would be eligible for 
   * death protection if the current conditions are met. Death Protection
   * guarantees that the user will survive fatal damage as long as the
   * damage is below a specified amount.*/
  void deathProtection(uint16_t &dmg_magnitude);

  bool guard_success = false;
private:
  bool can_parry;
  uint16_t boundary;

  Texture* charge_sprite;
  Texture* guard_sprite;
  Texture* parry_sprite;
};
