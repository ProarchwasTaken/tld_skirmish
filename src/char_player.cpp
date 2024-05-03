// char_player.cpp
#include <raylib.h>
#include "color_palette.h"
#include "char_player.h"
#include <plog/Log.h>


PlayerCharacter::PlayerCharacter() {
  PLOGI << "Initializing the player character.";
  position = {213, 208};
  hitbox_scale = {32, 64};
  tex_scale = {64, 64};
}

void PlayerCharacter::hitboxCorrection() {
  float x = position.x - hitbox_scale.x / 2;
  float y = position.y - hitbox_scale.y;

  hitbox_position = {x, y};
  hitbox = {x, y, hitbox_scale.x, hitbox_scale.y};
}

void PlayerCharacter::texRectCorrection() {
  float x = position.x - tex_scale.x / 2;
  float y = position.y - tex_scale.y;

  tex_position = {x, y};
  tex_rect = {x, y, tex_scale.x, tex_scale.y};
}

void PlayerCharacter::draw() {
  DrawRectangleLinesEx(tex_rect, 2, COLORS::PALETTE[10]);
  DrawRectangleLinesEx(hitbox, 2, COLORS::PALETTE[26]);
}
