// scene_index.h
#pragma once
#include "base/scene.h"
#include "hud_menu.h"
#include "game.h"


class IndexScene : public Scene {
public:
  IndexScene(Game &skirmish);

  void updateScene() override;
  void transitionLerp();

  void checkInput() override;
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);

  bool ready = false;
  bool exiting = false;

  float ready_time = 0.25;
  float ready_percentage = 0.0;

  float box_y = 69;
  Color box_tint = {255, 255, 255, 0};
};
