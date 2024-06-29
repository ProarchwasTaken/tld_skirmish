// cmd_guard.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <vector>
#include "base/combatant.h"
#include "base/action_command.h"
#include "sprite_loader.h"


class Guard : public ActionCommand {
public:
  Guard(Combatant *user, std::vector<SpriteMetaData> &data_list,
        bool can_parry = false);

  void chargeSequence(float time_elapsed, double &delta_time) override;
  void actSequence(float time_elapsed, double &delta_time) override;
  void recoverySequence(float time_elapsed, double &delta_time) override;

  void guardLogic(uint16_t &dmg_magnitude, float guard_pierce, 
                  float stun_time, float kb_velocity, 
                  uint8_t kb_direction);

  bool guard_success = false;
private:
  bool can_parry;

  Texture* charge_sprite;
  Texture* guard_sprite;
  Texture* parry_sprite;
};
