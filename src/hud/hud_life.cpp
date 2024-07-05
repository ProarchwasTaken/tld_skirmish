// hud/hud_life.h
#include <cmath>
#include <raylib.h>
#include "globals.h"
#include "char_player.h"
#include "hud_life.h"
#include <plog/Log.h>


LifeHud::LifeHud(PlayerCharacter *player) {
  this->player = player;

  hud_position = {16, 212};
  text_position = {0, hud_position.y - 2};

  gauge_position = {hud_position.x + 8, hud_position.y + 9};
  gauge_width = 69;
  gauge_source = {0, 0, gauge_width, 9};

  segment_width = gauge_width / 10;
  PLOGI << "Initialized LifeHud.";
}

void LifeHud::updateGauge() {
  float health = player->health;
  float life_percentage = health / player->max_health;

  int segments = ceil(life_percentage * 10);
  gauge_source.width = segment_width * segments;
}

void LifeHud::alignText(const char* health_text) {
  int size = fonts::skirmish->baseSize;
  int width = MeasureTextEx(*fonts::skirmish, health_text, size, -3).x;

  text_position.x = (hud_position.x + 79) - width;
}

void LifeHud::draw() {
  DrawTextureV(*sprites::hud_life[0], hud_position, WHITE);
  DrawTextureRec(*sprites::hud_life[1], gauge_source, gauge_position, 
                 WHITE);

  auto *txt_hp = TextFormat("%i/%i", player->health, player->max_health);
  alignText(txt_hp);

  int size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, txt_hp, text_position, size, -3, WHITE);
}
