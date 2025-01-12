// cmd_gun_light.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/action_command.h"
#include "base/combatant.h"
#include "char_player.h"


/* Light technique that is associated with the Gun SubWeapon. Capable of
 * inflicting continuous damage to a single enemy. In exchange the 
 * player's Morale would be continuously drained. 
 *
 * Visually, the player will fire probes from the stun gun, electrocuting
 * the first enemy it hits. This could go on for as long as the player 
 * holds the button.This action command really pushes the limits of what 
 * could be done.*/
class GunLight : public ActionCommand {
public:
  GunLight(PlayerCharacter *player);
  ~GunLight();

  void chargeSequence(float time_elapsed) override;

  /* The root function for deciding if the stun gun's probes has 
   * successful attached to an enemy. Should it only be called one time,
   * and thats when the charge sequence ends.*/
  void enemyHitCheck();

  /* Helper function for gathering a list of enemies, whose hitbox 
   * intersects with a long hurtbox that's scaled and positioned to be in
   * front of the player.*/
  void enemyDetection(std::vector<Combatant*> *detected_enemies);
  Rectangle setupHurtbox();

  /* For the scenario of which the number of enemies detected is more 
   * than one. Basically finds the closest enemy to the player through 
   * the use of an algorithm.*/
  void probeClosestEnemy(std::vector<Combatant*> &detected_enemies);

  /* There are two ways of which the act sequence could play out, and the
   * deciding factor is whether they are "stalling" it or not. In order
   * for the act phase to be stalled, two conditions must be met. One of
   * those conditions being whether the gun's probes had successfully
   * attached to an enemy.*/
  void actSequence(float time_elapsed) override;

  /* Returns true if the function detects that the button for performing
   * a Light Technique is held down at the time of which it is called. 
   * (Usually the A Key, or the left face button on most controls.) The
   * second condition that must be met in order for the act phase to be 
   * stalled.*/
  bool techInputHeldDown();

  /* Is always called when the player is about to move on to the recovery
   * phase for one reason or another. This should not be confused with
   * getting interrupted by getting sent into hit stun, or canceling the
   * action_command into another one.*/
  void wrapup();

  /* Forcefully moves the player onto the recovery phase regardless of 
   * whether they were holding the button down.*/
  void detachProbes();

  /* The one function that allows the player to slowly move around. Also
   * one of four major functions that will be called every frame of which
   * the player is stalling the act sequence.*/
  void slowMovement();

  /* Periodically makes the probed_enemy take damage at a pre-defined 
   * rate. I feel I don't need to explain why this function shouldn't be
   * called while probed_enemy is NULL.*/
  void tickDamage();

  /* The rate of which the player's morale deceases differs from the rate
   * of which the probed_enemy takes damage. The gun's probes will
   * automatically detach if the player's morale gets too low.*/
  void moraleDrain();

  /* Checks the distance between the player, and the probed_enemy. 
   * Automatically makes the gun's probes detach if the distance is too
   * great. Really only comes into play if the player moves too far from
   * the enemy, or the enemy moves too far from the player.*/
  void rangeCheck();

  bool hit_enemy = false;
private:
  combatant_list *enemies;
  PlayerCharacter *player;

  float max_range = 128;
  Combatant *probed_enemy = NULL;

  float tick_time = 0.75;
  float tick_timestamp = 0;
  uint8_t tick_damage = 2; 

  float drain_time = 0.35;
  float drain_timestamp = 0;
  uint8_t drain_magnitude = 2;

  std::vector<int> anim_slow = {29, 30, 31, 30};
  float slow_frametime = 0.15;
};
