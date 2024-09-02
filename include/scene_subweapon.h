// scene_subweapon.h
#pragma once
#include <raylib.h>
#include <functional>
#include "base/scene.h"


class SubWeaponScene : public Scene {
public:
  SubWeaponScene(std::function<void(int)> load_scene);
  void drawScene() override;
};
