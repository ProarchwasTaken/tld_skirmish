// action_command/cmd_guard.cpp
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
             bool can_parry):
  ActionCommand(user, "Guard", 0.15, 0.5, 0.25) 
{
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
    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);
    return;
  }

  float reduced_damage = dmg_magnitude * guard_pierce;
  dmg_magnitude = static_cast<int>(reduced_damage);
  PLOGI << "Reduced incoming damage to: " << dmg_magnitude;

  bool guard_failed = guard_pierce > user->stability;
  if (guard_failed) {
    PLOGI << "The attack has pierced the guard of: " << user->name;
    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);

    SoundUtils::play("guard_fail");
    return;
  }

  PLOGI << "Guard Successful! Now applying bonuses to: " << user->name;

  guard_success = true;
  user->invulnerable = true;

  stun_time = stun_time * 0.75;
  kb_velocity = kb_velocity * 0.75;

  recovery_time = stun_time;
  user->setKnockback(kb_velocity, kb_direction);

  SoundUtils::play("guard_success");
}
