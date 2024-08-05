// base/dynamic_actor.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "actor.h"

#define TYPE_PROJECTILE 0
#define TYPE_PARTICLE_FX 1


/* A "Dynamic Actor" is a game object that's capable of being created
 * from anywhere in the program through the call of a simple template
 * function. Used as the foundation for projectiles, and particle 
 * effects. Basically anything that doesn't fit the description of a
 * "Combatant."*/
class DynamicActor : public Actor {
public:
  DynamicActor(Vector2 position, uint8_t type, 
               Vector2 hitbox_scale = {16, 16}, 
               Vector2 tex_scale = {16, 16}, 
               Vector2 hitbox_offset = {-8, -8}, 
               Vector2 tex_offset = {-8, -8});

  virtual void update() {};

  uint8_t type;
  bool awaiting_deletion = false;
};
