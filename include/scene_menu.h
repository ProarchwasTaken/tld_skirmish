// scene_menu.h
#pragma once
#include <raylib.h>
#include <triblib/base/generics.h>
#include <triblib/base/scene.h>
#include "hud_menu.h"
#include "hud_menu_btn.h"
#include "game.h"

#define OPT_PLAY 0
#define OPT_SETTINGS 1
#define OPT_CONTROLS 2
#define OPT_INDEX 3
#define OPT_QUIT 4


/* The Main Menu is the nexus point for almost every other menu in the
 * game as all main menus tend to do. This scene makes use of the menu
 * utility functions, and it would be the 3rd scene loaded under normal
 * circumstances.*/
class MenuScene : public Scene {
public:
  MenuScene(Game &skirmish);
  MenuScene(Game &skirmish, MenuHud &menu_hud);
  ~MenuScene();

  void updateScene() override;

  void selectOption();
  void drawOptionDescription();

  void checkInput() override;
  void drawScene() override;

private:
  MenuHud menu_hud = MenuHud(true);
  bool exiting_scene = false;

  menu_options options = {
    OPT_PLAY,
    OPT_SETTINGS,
    OPT_CONTROLS,
    OPT_INDEX,
    OPT_QUIT
  };
  menu_options_txt options_text = {
    "PLAY",
    "SETTINGS",
    "CONTROLS",
    "INDEX",
    "QUIT"
  };

  menu_options::iterator selected_option = options.begin();
  MenuButtonsHud menu_btns = MenuButtonsHud(options, options_text, 
                                            selected_option, &menu_hud);
};
