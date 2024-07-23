// hud/hud_morale.cpp
#include <raylib.h>
#include <string>
#include "globals.h"
#include "utils.h"
#include "char_player.h"
#include "hud_morale.h"
#include <plog/Log.h>

using std::string;


MoraleHud::MoraleHud(PlayerCharacter &player) {
  this->player = &player;
  hud_position = {98, 207};

  PLOGI << "Initialized Morale hud.";
}

void MoraleHud::drawMoraleText() {
  string txt_mp = TextFormat("%02i/%02i", player->morale, 
                             player->max_morale);
  Vector2 base_position = {hud_position.x + 79, hud_position.y - 1};
  txt_position = Text::alignRight(fonts::skirmish, txt_mp, base_position,
                                  1, -3);

  int size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, txt_mp.c_str(), txt_position, size, -3, 
             COLORS::PALETTE[42]);
}

void MoraleHud::draw() {
  DrawTextureV(*sprites::hud_morale[0], hud_position, WHITE);
  drawMoraleText();
}
