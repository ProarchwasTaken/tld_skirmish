// cmd_damned_grb.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <vector>
#include "combatants/enemy_damned.h"
#include "combatants/char_player.h"
#include "base/action_command.h"


/* If the Damned could be considered the most complicated enemy in the
 * game in terms of programming, then DamnedGrab would probably be the
 * ActionCommand equivalent. I've recently came to realize that the 
 * enemy and respective their action command are like two halfs of the 
 * same whole.
 *
 * This ActionCommand is exclusive to the DamnedEnemy if you haven't
 * figured it out. It's an unblockable attack that upon hitting, will
 * cause the player to be grabbed, and dealt continuous damage until
 * the recovery phase ends, or they successfully struggle out of the
 * enemy's grasp.*/
class DamnedGrab : public ActionCommand {
public:
  DamnedGrab(DamnedEnemy *user);
  ~DamnedGrab();

  void chargeSequence(float time_elapsed) override;
  void setupHurtbox();

  /* If this ActionCommand is initialized while the enemy is
   * "crashing out" various adjustments will be made so the charge
   * sequence will function slightly differently.*/
  void performAdjustments();

  /* While crashing out, enemy will slowly decelerate to a halt during
   * the charge phase. This method makes this functionality possible.*/
  void decelerate();

  void actSequence(float time_elapsed) override;

  /* The first step in the sequence would be checking if the hurtbox
   * collides with the player's hitbox. Basically every other enemy
   * ActionCommand has a method like this, and this is where the grabCheck
   * is made if certain conditions are met.*/
  void playerHitCheck();

  /* The main method for checking if the enemy has successfully grabbed
   * the player. The results determines the rest of this ActionCommand's
   * functionality.*/
  bool grabCheck();

  /* Should only be called after the enemy has successfully grabbed the
   * player. Basically repositions the player to be in a certain distance
   * in front of the Damned to match up with a certain sprite it uses.*/
  void repositionPlayer();

  void recoverySequence(float time_elapsed) override;

  /* Responsible for having the player continuously take damage at a set 
   * rate. Should only be called while if the enemy has successfully 
   * grabbed the player.*/
  void tickDamage();

  /* This method is for allowing the player to break free from being 
   * grabbed early by mashing buttons. There's a input delay to prevent
   * players from using macros or turbo buttons.*/
  void struggleCheck();

  /* For ending the recovery sequence early when the damage the
   * player is taking while grabbed is starting to be a little "overkill"
   * so to speak.*/
  void overkillCheck();

  /* This could be refered as a "wrap up" method that called right before
   * the ActionCommand ends, and DamnedEnemy is set back to neutral. Reset
   * the DamnedEnemy's cooldown_patience to a random value. The range is
   * determined by a couple of factors.*/
  void resetCooldown();

  void drawDebug() override;

  // Never forget to define your variables kids.
  bool grabbed_player = false;
private:
  PlayerCharacter *player;
  Rectangle hurtbox;

  bool crashing_out = false;
  float max_momentum = 4.0;
  int8_t momentum_direction = 0;
  float percentage = 1.0;

  float tick_damage = 2;
  float tick_time = 1.5;
  float tick_timestamp;

  uint8_t progress = 0;
  uint8_t threshold = 20;

  float input_delay = 0.05;
  float input_timestamp = 0;

  float grab_time = 10.0;
  float stun_time;

  std::vector<int> anim_charge = {6, 7};
  float charge_frametime = 0.4;
};
