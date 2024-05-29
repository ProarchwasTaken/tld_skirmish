// actor.cpp
#include <raylib.h>
#include "base/actor.h"


Actor::Actor(Vector2 position, Vector2 hitbox_scale, Vector2 tex_scale) {
  this->position = position;
  this->hitbox_scale = hitbox_scale;
  this->tex_scale = tex_scale;

  hitboxCorrection();
  texRectCorrection();
}

void Actor::hitboxCorrection() {
  float x = position.x - hitbox_scale.x / 2;
  float y = position.y - hitbox_scale.y;

  hitbox_position = {x, y};
  hitbox = {x, y, hitbox_scale.x, hitbox_scale.y};
}

void Actor::texRectCorrection() {
  float x = position.x - tex_scale.x / 2;
  float y = position.y - tex_scale.y;

  tex_position = {x, y};
  tex_rect = {x, y, tex_scale.x, tex_scale.y};
}

void Actor::drawDebug() { 
  DrawRectangleLinesEx(tex_rect, 1, YELLOW);
  DrawRectangleLinesEx(hitbox, 1, BLUE);
}
