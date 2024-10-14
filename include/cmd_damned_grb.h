// cmd_damned_grb.h
#pragma once
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
  void resetCooldown();

  void drawDebug() override;
private:
  PlayerCharacter *player;
  Rectangle hurtbox;

  // Never forget to define your variables kids.
  bool grabbed_player = false;

  float grab_time = 10.0;
  float stun_time;

  std::vector<int> anim_charge = {6, 7};
  float charge_frametime = 0.4;
};
