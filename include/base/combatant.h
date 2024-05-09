// combatant.h
#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <raylib.h>
#include "generics.h"
#include "actor.h"

#define TYPE_PLAYER 0
#define TYPE_ENEMY 1

#define NEUTRAL 0
#define CHARGING 1
#define ACT 2
#define RECOVER 3
#define HIT_STUN 4


/* Everything, including the player, enemies, and bosses derive from this
 * class. Sets the ground work for the "fighting" aspect of the game,
 * (Which is basically all of it.) so the implementation must be done
 * correctly. Every combatant will have a name and type that will be used
 * for identification.*/
class Combatant : public Actor {
public:
  Combatant(std::string name, uint8_t type, uint16_t max_health,
            Vector2 position, Vector2 hitbox_scale = {32, 64},
            Vector2 tex_scale = {64, 64});

  std::string name;
  uint16_t max_health;
  uint8_t type;

  uint16_t health;
  uint8_t state;

  std::unique_ptr<ActionCommand> current_command;
};
