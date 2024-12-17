// scene_control.h
#pragma once
#include "base/scene.h"
#include "hud_menu.h"
#include "game.h"


class ControlScene : public Scene {
public:
  ControlScene(Game &skirmish);
  void checkInput() override;
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);
};
