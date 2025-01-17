// cmd_gun_heavy.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include <triblib/base/generics.h>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


/* Heavy Technique that's associated with the Gun SubWeapon. When 
 * performed, the player will charge and shoot a bolt of electricity, 
 * damaging all enemies that get hit by it. The longer it's charged, the
 * more range and damage it has, whilst draining morale. While it is more
 * simple then GunLight, this ActionCommand dabbles in using multiple 
 * animations for the player.*/
class GunHeavy : public ActionCommand {
public:
  GunHeavy(PlayerCharacter *player, uint8_t start_level);
  ~GunHeavy();

  /* Basically where most of the ActionCommand's functionality takes 
   * place. It all depends on whether the player has the heavy technique
   * button held down.*/
  void chargeSequence(float time_elapsed) override;
  bool techInputHeldDown();

  /* Increments the gun's charge level at a predefined rate, up until it
   * reaches it's limit.*/
  void charge();

  /* For deciding what animation the player should use while charging.
   * This is depended on the current charge level.*/
  void chargeAnimation();

  /* Works just as you would expect. If the player's morale is too low,
   * they will automatically shoot the gun.*/
  void moraleDrain();

  void shootAnimation();
  Rectangle setupHurtbox();

  void shoot();

  void recoverySequence(float time_elapsed) override;
private:
  combatant_list *enemies;

  PlayerCharacter *player;
  SubWeapon *sub_weapon;

  bool input_released = false;

  uint8_t level;
  uint8_t max_level = 3;

  float level_interval = 1.0;
  float level_timestamp;

  float drain_time = 0.45;
  float drain_timestamp = 0;

  uint8_t min_damage = 4;
  float min_range = 64;
  float min_stuntime = 0.5;

  std::vector<int> anim_charge = {34, 33};
  std::vector<int> anim_altcharge = {36, 35};
  float charge_frametime = 0.05;

  std::vector<int> anim_shoot = {37, 38};
  std::vector<int> anim_altshoot = {39, 40};
  float shoot_frametime = 0.10;
};
