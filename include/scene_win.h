// scene_win.h
#pragma once
#include <string>
#include <raylib.h>
#include "base/scene.h"
#include "game.h"


class WinScene : public Scene {
public:
  WinScene(Game &skimish);

  void updateScene() override;
  void interpolateAlpha();

  void drawScene() override;
private:
  std::string text = "CLEAR!";
  Vector2 position = {213, 104};
  Color txt_color;

  float fade_time = 1.0;
  float percentage = 1.0;

  float scene_time = 5.0;
  float scene_timestamp;
};
