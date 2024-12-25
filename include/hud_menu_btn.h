// hud_menu_btn.h
#pragma once
#include "base/generics.h"
#include "hud_menu.h"


class MenuButtonsHud {
public:
  MenuButtonsHud(menu_options &options, menu_options_txt &text,
                 menu_options::iterator &selected_option, 
                 MenuHud *menu_hud);

  void update();
  void syncSpade();

  void drawCursor(Vector2 current);
  void drawMenuButtons();
private:
  menu_options *options;
  menu_options_txt *options_text;
  menu_options::iterator *selected_option;

  MenuHud *menu_hud;

  bool syncing_spade = true;

  Color hud_tint = {255, 255, 255, 0};
  float base_x = 0;
};
