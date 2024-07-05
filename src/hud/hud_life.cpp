// hud/hud_life.h
#include <cmath>
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
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

void LifeHud::update() {
  updateGauge();
  segmentBlinkInterval();
  determineHudColor();
}

void LifeHud::determineHudColor() {
  switch (player->state) {
    case HIT_STUN: {
      hud_color = COLORS::PALETTE[22];
      return;
    }
    case DEAD: {
      hud_color = COLORS::PALETTE[32];
      return;
    } 
    default: {
      hud_color = WHITE;
    }
  }
}

void LifeHud::segmentBlinkInterval() {
  if (blink_interval == 0.0) {
    display_segment = false;
    return;
  }

  float time_elasped = GetTime() - blink_timestamp;
  if (time_elasped >= blink_interval) {
    display_segment = !display_segment;
    blink_timestamp = GetTime();
  }
}

void LifeHud::updateGauge() {
  float health = player->health;
  life_percentage = health / player->max_health;

  int base_value = floor(life_percentage * 10);
  blink_interval = (life_percentage * 10) - base_value;

  int segments = base_value;
  gauge_source.width = segment_width * segments;

  segment_source = {segment_width * segments, 0, segment_width, 9};
  segment_position = {
    gauge_position.x + (segment_width * segments), gauge_position.y
  };
}

void LifeHud::alignText(const char* health_text) {
  int size = fonts::skirmish->baseSize;
  int width = MeasureTextEx(*fonts::skirmish, health_text, size, -3).x;

  text_position.x = (hud_position.x + 79) - width;
}

void LifeHud::draw() {
  DrawTextureV(*sprites::hud_life[0], hud_position, WHITE);
  DrawTextureRec(*sprites::hud_life[1], gauge_source, gauge_position, 
                 hud_color);

  if (display_segment) {
    DrawTextureRec(*sprites::hud_life[1], segment_source, 
                   segment_position, hud_color);
  }

  drawLifeText();
}

void LifeHud::drawLifeText() {
  auto *txt_hp = TextFormat("%i/%i", player->health, player->max_health);
  alignText(txt_hp);

  int size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, txt_hp, text_position, size, -3, 
             hud_color);
}
