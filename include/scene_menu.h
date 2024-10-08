// scene_menu.h
#pragma once
#include <raylib.h>
#include "base/generics.h"
#include "base/scene.h"
#include "game.h"

#define OPT_PLAY 0
#define OPT_SETTINGS 1
#define OPT_CONTROLS 2
#define OPT_QUIT 3


/* The Main Menu is the nexus point for almost every other menu in the
 * game as all main menus tend to do. This scene makes use of the menu
 * utility functions, and it would be the 3rd scene loaded under normal
 * circumstances.*/
class MenuScene : public Scene {
public:
  MenuScene(Game &skirmish);
  ~MenuScene();

  void drawMenuOptions();
  void selectOption();
  void drawOptionDescription();

  void checkInput() override;
  void drawScene() override;

private:
  menu_options options = {
    OPT_PLAY,
    OPT_SETTINGS,
    OPT_CONTROLS,
    OPT_QUIT
  };
  menu_options_txt options_text = {
    "PLAY GAME",
    "SETTINGS",
    "CONTROLS",
    "QUIT GAME"
  };

  menu_options::iterator selected_option = options.begin();
};
