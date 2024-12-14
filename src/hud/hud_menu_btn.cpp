// hud/hud_menu_btn.cpp
#include <cstdint>
#include <cmath>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/generics.h"
#include "hud_menu_btn.h"

using std::string;


MenuButtonsHud::MenuButtonsHud(menu_options &options, 
                               menu_options_txt &text,
                               menu_options::iterator &selected_option)
{
  this->options = &options;
  this->options_text = &text;
  this->selected_option = &selected_option;
}

void MenuButtonsHud::drawCursor(Vector2 current) {
  float offset = cosf(CURRENT_TIME * 15) * 0.75;
  Vector2 position = Vector2Add(current, {62 + offset, 0});
  
  DrawTextureV(*sprites::hud_mainmenu[2], position, WHITE);
}

void MenuButtonsHud::drawMenuButtons() {
  int size = fonts::skirmish->baseSize;
  Vector2 current = {16, 53};

  for (uint8_t option : *options) {
    string *text = &options_text->at(option);
    Vector2 txt_position = Vector2Add(current, {2, -1});

    Color color;
    if (**selected_option == option) {
      color = COLORS::PALETTE[42];
      drawCursor(current);
    }
    else {
      color = WHITE;
    }

    DrawTextureV(*sprites::hud_mainmenu[1], current, WHITE);
    DrawTextEx(*fonts::skirmish, text->c_str(), txt_position, size, -3, 
               color);

    current.x += 2;
    current.y += 15;
  }

}
