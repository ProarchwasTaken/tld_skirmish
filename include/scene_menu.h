// scene_menu.h
#pragma once
#include <raylib.h>
#include <functional>
#include "base/generics.h"
#include "base/scene.h"

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
  MenuScene(std::function<void(int)> load_scene);
  ~MenuScene();

  void drawMenuOptions();
  void selectOption();
  void drawOptionDescription();

  void checkInput() override;
  void drawScene() override;

private:
  menu_options options;
  menu_options_txt options_text;
  menu_options::iterator selected_option;
};
