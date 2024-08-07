// actor.cpp
#include <cassert>
#include <raylib.h>
#include "base/actor.h"


Actor::Actor(Vector2 position, Vector2 hitbox_scale, Vector2 tex_scale,
             Vector2 hitbox_offset, Vector2 tex_offset) {
  this->position = position;

  this->hitbox_scale = hitbox_scale;
  this->tex_scale = tex_scale;

  this->hitbox_offset = hitbox_offset;
  this->tex_offset = tex_offset;

  hitboxCorrection();
  texRectCorrection();
}

void Actor::hitboxCorrection() {
  float x = position.x + hitbox_offset.x;
  float y = position.y + hitbox_offset.y;

  hitbox_position = {x, y};
  hitbox = {x, y, hitbox_scale.x, hitbox_scale.y};
}

void Actor::texRectCorrection() {
  float x = position.x + tex_offset.x;
  float y = position.y + tex_offset.y;

  tex_position = {x, y};
  tex_rect = {x, y, tex_scale.x, tex_scale.y};
}

void Actor::draw(Vector2 &camera_target) {
  assert(current_sprite != NULL && 
         "You forgot to set the default sprite idiot!");
}

void Actor::drawDebug() { 
  DrawRectangleLinesEx(tex_rect, 1, YELLOW);
  DrawRectangleLinesEx(hitbox, 1, BLUE);
}
