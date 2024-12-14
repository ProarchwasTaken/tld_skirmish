// hud_menu_btn.h
#pragma once
#include "base/generics.h"


class MenuButtonsHud {
public:
  MenuButtonsHud(menu_options &options, menu_options_txt &text,
                 menu_options::iterator &selected_option);

  void drawCursor(Vector2 current);
  void drawMenuButtons();
private:
  menu_options *options;
  menu_options_txt *options_text;
  menu_options::iterator *selected_option;
};
