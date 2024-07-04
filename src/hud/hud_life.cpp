// hud/hud_life.h
#include <raylib.h>
#include "globals.h"
#include "char_player.h"
#include "hud_life.h"
#include <plog/Log.h>


LifeHud::LifeHud(PlayerCharacter *player) {
  this->player = player;

  hud_position = {16, 212};
  PLOGI << "Initialized LifeHud.";
}

void LifeHud::draw() {
  DrawTextureV(*sprites::hud_life[0], hud_position, WHITE);
}
