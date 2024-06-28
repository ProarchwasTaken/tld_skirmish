// combatant.cpp
#include <cstdint>
#include <string>
#include <memory>
#include <raylib.h>
#include "base/actor.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils.h"
#include <plog/Log.h>

using std::string, std::unique_ptr;


Combatant::Combatant(string name, uint8_t type, uint16_t max_health,
                     Vector2 position, Vector2 hitbox_scale,
                     Vector2 tex_scale, Vector2 hitbox_offset,
                     Vector2 tex_offset):
Actor(position, hitbox_scale, tex_scale, hitbox_offset, tex_offset) 
{
  this->name = name;
  this->type = type;

  this->max_health = max_health;
  health = max_health;

  state = NEUTRAL;
  direction = RIGHT;
}

void Combatant::useCommand(unique_ptr<ActionCommand> &command) {
  if (current_command != nullptr) {
    current_command.reset();
  }

  current_command.swap(command);
  command.reset();
}

void Combatant::cancelCommand() {
  if (current_command == nullptr) {
    return;
  }

  PLOGI << name << " Combatant's action command has been canceled!";
  current_command.reset();

  if (state != HIT_STUN) {
    state = NEUTRAL;
  }
}

void Combatant::commandSequence() {
  if (current_command == nullptr) {
    PLOGF << name << "Combatant has attempted to go through command"
      << " sequence when no command is assigned to them!";
    throw;
  }

  float time_elapsed = GetTime() - current_command->sequence_timestamp;

  switch (state) {
    case CHARGING: {
      current_command->chargeSequence(time_elapsed);
      break;
    }
    case ACT: {
      current_command->actSequence(time_elapsed);
      break;
    }
    case RECOVER: {
      current_command->recoverySequence(time_elapsed);
      break;
    }
  }
}

void Combatant::takeDamage(uint16_t dmg_magnitude, float stun_time,
                           float kb_velocity, uint8_t kb_direction) 
{
  if (invulnerable) {
    return;
  }

  PLOGD << dmg_magnitude << " points of damage is being inflicted to "
    "combatant: " << name;
  SoundUtils::play("damage");

  int destined_health = health - dmg_magnitude;
  if (destined_health < 0) {
    destined_health = 0;
  }

  health = destined_health;
  PLOGI << "Combatant's health is now at: " << health;
 
  if (stun_time != 0) {
    setKnockback(kb_velocity, kb_direction);
    enterHitStun(stun_time);
    return;
  }

  if (health <= 0 && state != HIT_STUN) {
    death();
  }
}

void Combatant::enterHitStun(float stun_time) {
  state = HIT_STUN;
  this->stun_time = stun_time;

  cancelCommand();

  stun_timestamp = GetTime();
}

void Combatant::setKnockback(float kb_velocity, uint8_t kb_direction) {
  bool different_direction = this->kb_direction == kb_direction;
  bool greater_velocity = this->kb_velocity < kb_velocity;

  if (greater_velocity || different_direction) {
    PLOGI << "Updating knockback velocity to: " << kb_velocity;
    // I hope the decision of having both lines within this if statement 
    // won't come back to bite me later.
    this->kb_velocity = kb_velocity;
    this->kb_direction = kb_direction;
  } 
}

void Combatant::applyKnockback(double &delta_time, uint16_t boundary) {
  float magnitude = (kb_velocity * kb_direction) * delta_time;

  if (magnitude == 0) {
    return;
  }
  
  float half_scaleX = hitbox_scale.x / 2;
  float offset = position.x + magnitude + (half_scaleX * direction);

  if (offset < -boundary) {
    position.x = -boundary + half_scaleX;
  }
  else if (offset > boundary) {
    position.x = boundary - half_scaleX;
  }
  else {
    position.x += magnitude;
  }

  hitboxCorrection();
  texRectCorrection();
}

void Combatant::death() {
  PLOGV << "{Combatant: " << name << "} is now dead!";
  cancelCommand();
  state = DEAD;

  SoundUtils::play("death");
  death_timestamp = GetTime();
}

void Combatant::stunSequence() {
  if (state != HIT_STUN) {
    PLOGF << "{Combatant: " << name << "} entered stun sequence when it"
      " wasn't supposed to!";
    throw;
  }

  float time_elapsed = GetTime() - stun_timestamp;
  if (time_elapsed < stun_time) {
    return;
  }

  PLOGD << "{Combatant: " << name << "} has now finished stun sequence";
  if (health > 0) {
    state = NEUTRAL;
    kb_velocity = 0;
  } 
  else {
    death();
  }
}
