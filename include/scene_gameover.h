// scene_gameover.h
#pragma once
#include <raylib.h>
#include <string>
#include "base/scene.h"
#include "game.h"

#define OPT_RESTART 0
#define OPT_TITLE 1
#define OPT_QUIT 2


class GameOverScene : public Scene {
public:
  GameOverScene(Game &skirmish, Image screenshot);
  ~GameOverScene();

  void setupBackground(Image &screenshot);

  void updateScene() override;
  void interpolateSpacing();

  void drawScene() override;
private:
  std::string text = "GAME OVER";
  Vector2 txt_position = {213, 32};

  float start_spacing = 426;
  int spacing = start_spacing;

  float percentage = 1.0;
  float lerp_time = 0.5;
  bool ready = false;

  Texture background;
  Rectangle bg_source;
  Rectangle bg_dest;
};
