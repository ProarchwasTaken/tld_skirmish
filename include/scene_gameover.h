// scene_gameover.h
#pragma once
#include <raylib.h>
#include <string>
#include "base/scene.h"
#include "game.h"


class GameOverScene : public Scene {
public:
  GameOverScene(Game &skirmish, Texture background);
  ~GameOverScene();

  void updateScene() override;
  void interpolateSpacing();

  void drawScene() override;
private:
  std::string text = "GAME OVER";
  Vector2 txt_position = {213, 32};

  float start_spacing = 100;
  float spacing = start_spacing;

  float percentage = 1.0;

  bool ready = false;
  float lerp_time = 0.5;

  Texture background;
};
