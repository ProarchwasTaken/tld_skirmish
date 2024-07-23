// hud/hud_morale.cpp
#include <raylib.h>
#include "globals.h"
#include "char_player.h"
#include "hud_morale.h"
#include <plog/Log.h>


MoraleHud::MoraleHud(PlayerCharacter &player) {
  this->player = &player;
  hud_position = {98, 208};

  PLOGI << "Initialized Morale hud.";
}

void MoraleHud::draw() {
  DrawTextureV(*sprites::hud_morale[0], hud_position, WHITE);
}
