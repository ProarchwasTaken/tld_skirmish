// cmd_guard.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include <vector>
#include "base/combatant.h"
#include "base/action_command.h"
#include "sprite_loader.h"

#define DEF_PARRY_WINDOW 0.20


class Guard : public ActionCommand {
public:
  Guard(Combatant *user, std::vector<SpriteMetaData> &data_list,
        uint16_t boundary, bool can_parry = false);

  void chargeSequence(float time_elapsed, double &delta_time) override;
  void actSequence(float time_elapsed, double &delta_time) override;
  void recoverySequence(float time_elapsed, double &delta_time) override;

  void guardLogic(uint16_t &dmg_magnitude, float guard_pierce, 
                  float stun_time, float kb_velocity, 
                  uint8_t kb_direction);

  bool guardFailed(float guard_pierce, float stun_time, float kb_velocity,
                   uint8_t kb_direction);

  void applyGuardBonus(float stun_time, float kb_velocity = 0, 
                       uint8_t kb_direction = 0);

  bool parriedAttack(float guard_pierce, float stun_time);

  void deathProtection(uint16_t &dmg_magnitude);

  bool guard_success = false;
private:
  bool can_parry;
  uint16_t boundary;

  Texture* charge_sprite;
  Texture* guard_sprite;
  Texture* parry_sprite;
};