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


class MenuScene : public Scene {
public:
  MenuScene(std::function<void(int)> load_scene);

private:
  menu_options options;
  menu_options::iterator selected_option;
};
