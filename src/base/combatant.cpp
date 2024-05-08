// combatant.cpp
#include <cstdint>
#include <string>
#include <raylib.h>
#include "base/combatant.h"
#include "base/actor.h"

using std::string;


Combatant::Combatant(string name, uint8_t type, uint16_t max_health,
                     Vector2 position, Vector2 hitbox_scale,
                     Vector2 tex_scale):
Actor(position, hitbox_scale, tex_scale) {
  this->name = name;
  this->type = type;

  this->max_health = max_health;
  health = max_health;
  state = NEUTRAL;
}
