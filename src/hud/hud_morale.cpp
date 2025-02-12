// hud/hud_morale.cpp
#include <raylib.h>
#include <string>
#include <cmath>
#include "globals.h"
#include <triblib/utils/text.h>
#include "combatants/char_player.h"
#include "hud/hud_morale.h"
#include <plog/Log.h>

using std::string;


MoraleHud::MoraleHud(PlayerCharacter &player) {
  this->player = &player;
  hud_position = {98, 208};
  
  gauge_width = 70;
  segment_width = gauge_width * 0.2;

  gauge_source = {0, 0, gauge_width, 6};
  gauge_position = {hud_position.x + 7, hud_position.y + 9};

  segment_source = {0, 0, segment_width, 6};
  segment_position.y = gauge_position.y;

  blink_timestamp = CURRENT_TIME;
  PLOGI << "Initialized Morale hud.";
}

void MoraleHud::update() {
  updateGauge();
  segmentBlinkInterval();

  if (player->sub_weapon->usable) {
    hud_color = COLORS::PALETTE[42];
  }
  else {
    hud_color = COLORS::PALETTE[2];
  }
}

void MoraleHud::updateGauge() {
  float morale = player->morale;
  morale_percentage = morale / player->max_morale;

  float percentage = morale_percentage / 0.20;
  int base_value = floor(percentage);
  gauge_source.width = segment_width * base_value;

  float remainder = percentage - base_value;
  blink_interval = 1.0 - remainder;

  segment_position.x = gauge_position.x + (segment_width * base_value);
}

void MoraleHud::segmentBlinkInterval() {
  float time_elapsed = CURRENT_TIME - blink_timestamp;
  if (time_elapsed >= blink_interval) {
    display_segment = !display_segment;
    blink_timestamp = CURRENT_TIME;
  }
}

void MoraleHud::drawMoraleText() {
  string txt_mp = TextFormat("%02i/%02i", player->morale, 
                             player->max_morale);
  Vector2 base_position = {hud_position.x + 78, hud_position.y - 2};
  txt_position = Text::alignRight(fonts::skirmish, txt_mp, base_position,
                                  1, -3);

  int size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, txt_mp.c_str(), txt_position, size, -3, 
             hud_color);
}

void MoraleHud::drawMoraleGauge() {
  DrawTextureV(*sprites::hud_morale[0], hud_position, hud_color);
  DrawTextureV(*sprites::hud_morale[3], gauge_position, WHITE);
  DrawTextureRec(*sprites::hud_morale[2], gauge_source, gauge_position, 
                 hud_color);

  if (blink_interval == 1.0) {
    return;
  }

  int index;
  if (display_segment) {
    index = 2;
  }
  else {
    index = 1;
  }

  DrawTextureRec(*sprites::hud_morale[index], segment_source, 
                 segment_position, hud_color);
}

void MoraleHud::draw() {
  drawMoraleGauge();
  drawMoraleText();
}
