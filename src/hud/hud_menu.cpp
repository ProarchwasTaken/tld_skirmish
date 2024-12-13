// hud/hud_menu.cpp
#include <raylib.h>
#include "globals.h"
#include "hud_menu.h"


MenuHud::MenuHud(bool interpolate) {
  this->interpolate = interpolate;
}

void MenuHud::drawTopBar() {
  DrawTexture(*sprites::hud_mainmenu[5], 0, 0, WHITE);
}

void MenuHud::draw() {
  drawTopBar();
}
