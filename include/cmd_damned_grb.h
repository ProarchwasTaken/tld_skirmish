// cmd_damned_grb.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <vector>
#include "enemy_damned.h"
#include "char_player.h"
#include "base/action_command.h"


class DamnedGrab : public ActionCommand {
public:
  DamnedGrab(DamnedEnemy *user);
  ~DamnedGrab();

  void chargeSequence(float time_elapsed) override;
  void setupHurtbox();

  void actSequence(float time_elapsed) override;
  void playerHitCheck();

  /* The main method for checking if the enemy has successfully grabbed
   * the player.The results determines the rest of this ActionCommand's
   * functionality.*/
  bool grabCheck();

  void repositionPlayer();

  void recoverySequence(float time_elapsed) override;
  void tickDamage();

  /* This method is for allowing the player to break free from being 
   * grabbed early by mashing buttons. There's a input delay to prevent
   * players from using macros or turbo buttons.*/
  void struggleCheck();
  void resetCooldown();

  void drawDebug() override;
private:
  PlayerCharacter *player;
  Rectangle hurtbox;

  // Never forget to define your variables kids.
  bool grabbed_player = false;

  float tick_damage = 2;
  float tick_time = 1.5;
  float tick_timestamp;

  uint8_t progress = 0;
  uint8_t threshold = 30;

  float input_delay = 0.05;
  float input_timestamp = 0;

  float grab_time = 10.0;
  float stun_time;

  std::vector<int> anim_charge = {6, 7};
  float charge_frametime = 0.4;
};
