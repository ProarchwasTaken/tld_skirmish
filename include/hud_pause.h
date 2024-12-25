// hud_pause.h
#pragma once
#include "base/generics.h"
#include "hud_menu.h"


class PauseHud : public MenuHud {
public:
  PauseHud(GameplayScene *scene);

  void restartOpening() override;
  void checkInput();

  void update() override;
  void draw() override;
private:
  GameplayScene *scene;
};
