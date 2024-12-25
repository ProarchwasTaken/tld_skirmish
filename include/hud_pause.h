// hud_pause.h
#pragma once
#include "base/generics.h"
#include "hud_menu.h"
#include "hud_menu_btn.h"

#define OPT_RESUME 0
#define OPT_TITLE 1
#define OPT_QUIT 2


class PauseHud : public MenuHud {
public:
  PauseHud(GameplayScene *scene);

  void restartOpening() override;

  void update() override;
  void selectOption();

  void checkInput();
  void draw() override;
private:
  GameplayScene *scene;

  bool closing_menu = false;

  menu_options options = {
    OPT_RESUME,
    OPT_TITLE,
    OPT_QUIT
  };

  menu_options_txt options_txt = {
    "RESUME",
    "TITLE",
    "QUIT"
  };

  menu_options::iterator selected_option = options.begin();
  MenuButtonsHud menu_btns = MenuButtonsHud(options, options_txt,
                                            selected_option, this);
};
