// scene_subweapon.h
#pragma once
#include <raylib.h>
#include "base/scene.h"
#include "game.h"


class SubWeaponScene : public Scene {
public:
  SubWeaponScene(Game &skirmish);
  void drawScene() override;
};
