// scenes/scene_menu.cpp
#include <raylib.h>
#include <functional>
#include "scene_menu.h"
#include <plog/Log.h>

using std::function;


MenuScene::MenuScene(function<void(int)> load_scene) : Scene(load_scene) {
  PLOGV << "Loading MainMenu scene.";
}
