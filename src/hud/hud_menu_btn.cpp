// hud/hud_menu_btn.cpp
#include <cstdint>
#include <cmath>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/generics.h"
#include "hud_menu.h"
#include "hud_menu_btn.h"

using std::string;
constexpr float START_HUD_X = -16;
constexpr float DEF_HUD_X = 16;


MenuButtonsHud::MenuButtonsHud(menu_options &options, 
                               menu_options_txt &text,
                               menu_options::iterator &selected_option,
                               MenuHud *menu_hud)
{
  this->options = &options;
  this->options_text = &text;
  this->selected_option = &selected_option;
  this->menu_hud = menu_hud;
}

void MenuButtonsHud::update() {
  if (menu_hud->spade_state != SPADE_STANDBY) {
    syncSpade();
    syncing_spade = true;
  }
  else if (syncing_spade) {
    syncSpade();
    syncing_spade = false;
  }
}

void MenuButtonsHud::syncSpade() {
  float percentage = menu_hud->spade_percentage;

  base_x = Lerp(START_HUD_X, DEF_HUD_X, percentage);
  hud_tint.a = Lerp(0, 255, percentage);
}

void MenuButtonsHud::drawCursor(Vector2 current) {
  float offset = cosf(CURRENT_TIME * 15) * 0.75;
  Vector2 position = Vector2Add(current, {62 + offset, 0});
  
  DrawTextureV(*sprites::hud_mainmenu[2], position, WHITE);
}

void MenuButtonsHud::drawMenuButtons() {
  int size = fonts::skirmish->baseSize;
  Vector2 current = {base_x, 53};

  for (uint8_t option : *options) {
    string *text = &options_text->at(option);
    Vector2 txt_position = Vector2Add(current, {2, -1});

    Color color = hud_tint;

    bool on_standby = menu_hud->spade_state == SPADE_STANDBY;
    if (on_standby && **selected_option == option) {
      color = COLORS::PALETTE[42];
      drawCursor(current);
    }

    DrawTextureV(*sprites::hud_mainmenu[1], current, hud_tint);
    DrawTextEx(*fonts::skirmish, text->c_str(), txt_position, size, -3, 
               color);

    current.x += 2;
    current.y += 15;
  }

}
