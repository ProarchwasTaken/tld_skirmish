// action_command/cmd_guard.cpp
#include <cstdint>
#include <raylib.h>
#include <vector>
#include "base/combatant.h"
#include "base/action_command.h"
#include "sprite_loader.h"
#include "utils.h"
#include "cmd_guard.h"
#include <plog/Log.h>

using std::vector;


Guard::Guard(Combatant *user, vector<SpriteMetaData> &data_list,
             uint16_t boundary, bool can_parry):
  ActionCommand(user, "Guard", 0.15, 0.5, 0.25) 
{
  this->boundary = boundary;

  charge_sprite = Sprite::getSprite("charge", data_list);
  guard_sprite = Sprite::getSprite("guard", data_list);

  this->can_parry = can_parry;
  if (can_parry) {
    parry_sprite = Sprite::getSprite("parry", data_list);
  }

  user->current_sprite = charge_sprite;
}

void Guard::chargeSequence(float time_elapsed, double &delta_time) {
  ActionCommand::chargeSequence(time_elapsed, delta_time);

  if (finished_charge) {
    user->current_sprite = guard_sprite;
  }
}

void Guard::actSequence(float time_elapsed, double &delta_time) {
  ActionCommand::actSequence(time_elapsed, delta_time);

  if (finished_action) {
    user->current_sprite = charge_sprite;
  }
}

void Guard::recoverySequence(float time_elapsed, double &delta_time) {
  ActionCommand::recoverySequence(time_elapsed, delta_time);

  if (guard_success) {
    user->applyKnockback(delta_time, boundary);
  }

  if (finished_recovering && guard_success) {
    user->invulnerable = false;
  }
}

void Guard::guardLogic(uint16_t &dmg_magnitude, float guard_pierce, 
                       float stun_time, float kb_velocity, 
                       uint8_t kb_direction)
{
  // TODO: this will do until it's time to implement parrying
  if (user->state != ACT) {
    PLOGI << "Guard failed due to the user not being in the active phase";
    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);
    return;
  }

  float reduced_damage = dmg_magnitude * guard_pierce;
  dmg_magnitude = static_cast<int>(reduced_damage);
  PLOGI << "Reduced incoming damage to: " << dmg_magnitude;

  bool guard_failed = guard_pierce > user->stability;
  if (guard_failed) {
    PLOGI << "Guard failed because the guard_pierce of the attack is " 
      "greater than user's stability.";

    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);

    SoundUtils::play("guard_fail");
    return;
  }

  PLOGI << "Guard Successful! Now applying bonuses.";
  bool death_protection = dmg_magnitude < 10;
  bool death_imminent = user->health - dmg_magnitude <= 0;

  if (death_protection && death_imminent) {
    PLOGV << user->name << " survives fatal damage due to being eligible"
      " for death protection";
    dmg_magnitude = 0;
    user->health = 1;
  }

  guard_success = true;
  user->invulnerable = true;

  stun_time = stun_time * 0.75;
  kb_velocity = kb_velocity * 1.25;

  user->setKnockback(kb_velocity, kb_direction);
  recovery_time = stun_time;

  SoundUtils::play("guard_success");
  user->state = RECOVER;
  sequence_timestamp = GetTime();
}
