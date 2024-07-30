// base/dynamic_actor.cpp
#include <cstdint>
#include <raylib.h>
#include "base/actor.h"
#include "base/dynamic_actor.h"


DynamicActor::DynamicActor(Vector2 position, uint8_t type, 
                           Vector2 hitbox_scale, Vector2 tex_scale,
                           Vector2 hitbox_offset, Vector2 tex_offset):
Actor(position, hitbox_scale, tex_scale, hitbox_offset, tex_offset) {
  this->type = type;
}
