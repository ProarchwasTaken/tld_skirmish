// combatant.cpp
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <raylib.h>
#include <cassert>
#include "globals.h"
#include "base/actor.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_guard.h"
#include "fx_dmg_number.h"
#include "utils.h"
#include <plog/Log.h>

using std::string, std::unique_ptr, std::vector;


Combatant::Combatant(string name, uint8_t type, uint16_t max_health,
                     float stability, Vector2 position, 
                     Vector2 hitbox_scale, Vector2 tex_scale, 
                     Vector2 hitbox_offset, Vector2 tex_offset):
Actor(position, hitbox_scale, tex_scale, hitbox_offset, tex_offset) 
{
  this->name = name;
  this->type = type;

  this->max_health = max_health;
  health = max_health;

  this->stability = stability;

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
  assert(current_command != nullptr && 
         "Combatant has attaempted to go through command sequence when"
         " no command is assigned to them!");

  float time_elapsed = CURRENT_TIME - current_command->sequence_timestamp;

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

bool Combatant::isUsingCommand() {
  switch (state) {
    case CHARGING:
    case ACT:
    case RECOVER: {
      return true;
    }
    default: {
      return false;
    }
  }
}

void Combatant::takeDamage(uint16_t dmg_magnitude, float guard_pierce,
                           float stun_time, float kb_velocity, 
                           uint8_t kb_direction) 
{
  if (invulnerable) {
    return;
  }

  PLOGI << dmg_magnitude << " points of damage is being inflicted to "
    "combatant: " << name;
  SoundUtils::play("damage");

  
  bool using_command = isUsingCommand();
  if (using_command && current_command->command_name == "Guard") {
    PLOGI << "Detected that {Combatant: " << name << "} is guarding."
      " performing guard logic.";
    Guard *command = dynamic_cast<Guard*>(current_command.get());
    command->guardLogic(dmg_magnitude, guard_pierce, stun_time, 
                        kb_velocity, kb_direction);
  }
  else if (stun_time != 0) {
    setKnockback(kb_velocity, kb_direction);
    enterHitStun(stun_time);
  }

  if (parried_attack) {
    return;
  }

  int destined_health = health - dmg_magnitude;
  Color num_color = WHITE;

  if (destined_health <= 0) {
    destined_health = 0;
    num_color = COLORS::PALETTE[32];
  }

  createDamageNumber(dmg_magnitude, num_color);
  health = destined_health;
  PLOGI << "Combatant's health is now at: " << health;

  if (state != HIT_STUN && health <= 0) {
    death();
  }
}

void Combatant::createDamageNumber(int value, Color color) {
  Vector2 spawn_pos = {position.x, position.y - tex_scale.y};
  Dynamic::create<DamageNumber>(value, spawn_pos, color);
}

void Combatant::enterHitStun(float stun_time) {
  state = HIT_STUN;
  this->stun_time = stun_time;
  cancelCommand();

  stun_timestamp = CURRENT_TIME;
}

void Combatant::setKnockback(float kb_velocity, uint8_t kb_direction) {
  bool different_direction = this->kb_direction != kb_direction;
  bool greater_velocity = this->kb_velocity < kb_velocity;

  if (greater_velocity) {
    PLOGD << "Updating knockback velocity to: " << kb_velocity;
    // I hope the decision of having both lines within this if statement 
    // won't come back to bite me later.
    this->kb_velocity = kb_velocity;
  } 

  if (different_direction && kb_direction != 0) {
    PLOGD << "Updated knockback direction.";
    this->kb_direction = kb_direction;
  }
}

void Combatant::applyKnockback(uint16_t boundary) {
  float magnitude = (kb_velocity * kb_direction) * DELTA_TIME;

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
  PLOGI << "{Combatant: " << name << "} is now dead!";
  cancelCommand();
  state = DEAD;

  SoundUtils::play("death");
  death_timestamp = CURRENT_TIME;
}

void Combatant::stunSequence() {
  assert(state == HIT_STUN && 
         "Method called while combatant is not in hit stun!");

  float time_elapsed = CURRENT_TIME - stun_timestamp;
  if (time_elapsed < stun_time) {
    return;
  }

  PLOGI << "{Combatant: " << name << "} has now finished stun sequence";
  if (health > 0) {
    state = NEUTRAL;

    kb_velocity = 0;
    kb_direction = 0;
  } 
  else {
    death();
  }
}

void Combatant::deathSequence(vector<Texture*> &sprite_list,
                              vector<int> &anim_death,
                              float death_frametime)
{
  Animation::play(this, sprite_list, anim_death, death_frametime, false);

  float time_elapsed = CURRENT_TIME - death_timestamp;
  bool end_of_animation = current_frame == current_anim->end();

  float death_time = death_frametime * anim_death.size();

  if (end_of_animation && time_elapsed >= death_time) {
    awaiting_deletion = true;
  }
}
