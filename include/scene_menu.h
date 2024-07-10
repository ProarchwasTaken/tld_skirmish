// scene_menu.h
#pragma once
#include <raylib.h>
#include <functional>
#include "base/scene.h"


class MenuScene : public Scene {
public:
  MenuScene(std::function<void(int)> load_scene);
};
