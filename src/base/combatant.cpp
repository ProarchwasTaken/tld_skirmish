// combatant.cpp
#include <cstdint>
#include <string>
#include <memory>
#include <raylib.h>
#include "base/actor.h"
#include "base/combatant.h"
#include "base/action_command.h"
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
    PLOGE << name << "Combatant doesn't have a action command assigned!";
    return;
  }

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

void Combatant::takeDamage(uint16_t dmg_magnitude, float stun_time) {
  PLOGD << dmg_magnitude << " points of damage is being inflicted to "
    "combatant: " << name;
  int destined_health = health - dmg_magnitude;
  if (destined_health < 0) {
    destined_health = 0;
  }

  PLOGI << "Setting the combatant's health from " << health << " to " <<
    destined_health;
  health = destined_health;

  cancelCommand();

  this->stun_time = stun_time;
  if (stun_time != 0) {
    state = HIT_STUN;
    stun_timestamp = GetTime();
    return;
  }

  if (health <= 0 && state != HIT_STUN) {
    death();
  }
}

void Combatant::death() {
  PLOGV << "{Combatant: " << name << "} is now dead!";
  state = DEAD;
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
  } 
  else {
    death();
  }
}
