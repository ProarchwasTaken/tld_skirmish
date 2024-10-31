// cmd_gun_light.h
#pragma once
#include <raylib.h>
#include <vector>
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"


class GunLight : public ActionCommand {
public:
  GunLight(PlayerCharacter *player);

  void setupHurtbox();
  void chargeSequence(float time_elapsed) override;

  void actSequence(float time_elapsed) override;
private:
  Rectangle hurtbox;
  combatant_list *enemies;
  PlayerCharacter *player;

  std::vector<int> anim_charge = {28, 29};
  float charge_frametime = 0.2;
};
