// scene_title.h
#pragma once
#include <raylib.h>
#include <functional>
#include "base/scene.h"


class TitleScene : public Scene {
public:
  TitleScene(std::function<void(int)> load_scene);
  void drawScene() override;
};
