// scenes/scene_menu.cpp
#include <raylib.h>
#include <functional>
#include "scene_menu.h"
#include <plog/Log.h>

using std::function;


MenuScene::MenuScene(function<void(int)> load_scene) : Scene(load_scene) {
  PLOGV << "Loading MainMenu scene.";
  options = {
    OPT_PLAY,
    OPT_SETTINGS,
    OPT_CONTROLS,
    OPT_QUIT
  };

  selected_option = options.begin();
}
