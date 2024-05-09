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
                     Vector2 tex_scale):
Actor(position, hitbox_scale, tex_scale) 
{
  this->name = name;
  this->type = type;

  this->max_health = max_health;
  health = max_health;
  state = NEUTRAL;
}

void Combatant::useCommand(unique_ptr<ActionCommand> command) {
  if (current_command != nullptr) {
    current_command.reset();
  }

  current_command.swap(command);
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
