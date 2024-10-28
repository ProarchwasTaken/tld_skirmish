// action_command/cmd_guard.cpp
#include <cassert>
#include <cmath>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils_sprite.h"
#include "utils_sound.h"
#include "char_player.h"
#include "cmd_guard.h"
#include <plog/Log.h>

using std::vector;


Guard::Guard(Combatant *user, vector<SpriteMetaData> &data_list,
             uint16_t boundary, bool can_parry):
  ActionCommand(user, "Guard", CMD_NONE, 0.25, 0.5, 0.25) 
{
  this->boundary = boundary;

  charge_sprite = Sprite::getSprite("charge", data_list);
  guard_sprite = Sprite::getSprite("guard", data_list);

  this->can_parry = can_parry;
  if (can_parry) {
    parry_sprite = Sprite::getSprite("parry", data_list);
  }

  if (user->type == TYPE_PLAYER) {
    act_time = 0.05;
  }
  user->current_sprite = charge_sprite;
}

void Guard::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->current_sprite = guard_sprite;
  }
}

void Guard::actSequence(float time_elapsed) {
  if (user->type == TYPE_PLAYER) {
    stallCheck();
  }

  ActionCommand::actSequence(time_elapsed);

  if (finished_action) {
    user->current_sprite = charge_sprite;
  }
}

void Guard::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);

  if (guard_success && user->parried_attack == false) {
    user->applyKnockback(boundary);
  }

  if (finished_recovering && user->parried_attack) {
    user->parried_attack = false;
  }

  if (finished_recovering && guard_success) {
    user->invulnerable = false;
  }
}

void Guard::stallCheck() {
  assert(user->type == TYPE_PLAYER);
  bool key_space = IsKeyDown(KEY_SPACE);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_shoulder = false;
  if (gamepad_detected) {
    btn_shoulder = IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) 
      || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) 
      || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1)
      || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
  }

  bool guard_held = key_space || btn_shoulder;
  if (guard_held) {
    sequence_timestamp = CURRENT_TIME;
  }
}

void Guard::guardLogic(uint16_t &dmg_magnitude, float guard_pierce, 
                       float stun_time, float kb_velocity, 
                       int8_t kb_direction)
{
  if (user->state == RECOVER || user->stability < 0) {
    PLOGI << "Guard failed due to the user being in the recovery phase or"
    " their stability is in the negatives.";
    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);
    return;
  }

  if (user->state == CHARGING && parriedAttack(guard_pierce, stun_time)) {
    SoundUtils::play("parry");

    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
    return;
  }
  else if (user->state == CHARGING) { 
    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);
    return;
  }

  float reduced_damage = dmg_magnitude * guard_pierce;
  dmg_magnitude = static_cast<int>(reduced_damage);
  PLOGI << "Reduced incoming damage to: " << dmg_magnitude;

  PLOGI << "Comparing the user's stability and the attack's guard pierce";
  bool failed = guardFailed(guard_pierce);

  if (failed) {
    PLOGI << "Guard failed because the guard_pierce of the attack is " 
      "greater than user's stability.";
    user->setKnockback(kb_velocity, kb_direction);
    user->enterHitStun(stun_time);
    SoundUtils::play("guard_fail");
  }
  else {
    PLOGI << "Guard Successful! Now applying bonuses.";
    deathProtection(dmg_magnitude);
    applyGuardBonus(stun_time, kb_velocity, kb_direction);

    SoundUtils::play("guard_success");

    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME; 
  }

  user->stability -= guard_pierce * 0.5;
}

bool Guard::guardFailed(float guard_pierce) {
  if (guard_pierce > user->stability) {
    return true;
  }
  else {
    return false;
  }
}

void Guard::applyGuardBonus(float stun_time, float kb_velocity,
                            int8_t kb_direction) 
{
  guard_success = true;
  user->invulnerable = true;

  if (user->type == TYPE_PLAYER) {
    auto player = static_cast<PlayerCharacter*>(user);

    int morale_bonus = 2 + (2 * player->parried_attack);
    player->incrementMorale(morale_bonus);
  }

  stun_time = stun_time * 0.75;
  kb_velocity = kb_velocity * 1.25;

  user->setKnockback(kb_velocity, kb_direction);
  recovery_time = stun_time;
}

bool Guard::parriedAttack(float guard_pierce, float stun_time) {
  if (can_parry == false) {
    PLOGI << "Combatant is not capable of parrying. Guard Failed.";
    return false;
  }

  float time_elapsed = CURRENT_TIME - sequence_timestamp;

  float subtrahend = (2 * pow(guard_pierce, 2)) / 5;
  float parry_window = DEF_PARRY_WINDOW - subtrahend;

  parry_window = Clamp(parry_window, 0.05, DEF_PARRY_WINDOW); 
  PLOGD << "Timeframe for a successful parry is: " << parry_window;

  if (time_elapsed <= parry_window) {
    PLOGI << user->name << " parried the attack! All damage nullified!";
    user->current_sprite = parry_sprite;
    user->parried_attack = true;

    user->stability += user->max_stability * 0.25;
    user->stability = Clamp(user->stability, 0, user->max_stability);

    applyGuardBonus(stun_time);
    return true;
  }
  else {
    PLOGI << "Timeframe has been missed. Guard failed.";
    return false;
  }
}

void Guard::deathProtection(uint16_t &dmg_magnitude) {
  bool death_protection = dmg_magnitude <= 5;
  bool death_imminent = user->health - dmg_magnitude <= 0;

  if (death_protection && death_imminent) {
    PLOGI << user->name << " survives fatal damage due to being eligible"
      " for death protection";
    dmg_magnitude = 0;
    user->health = 1;
  }
}
