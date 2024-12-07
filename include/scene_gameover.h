// scene_gameover.h
#pragma once
#include <raylib.h>
#include <string>
#include "base/generics.h"
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

  void exitSequence();
  void endExitSequence();

  void interpolateSpacing();
  void interpolateZoom();
  void interpolateAlpha();

  void checkInput() override;
  void selectOption();
  void drawMenuOptions();

  void drawScene() override;
private:
  bool ready = false;
  bool exiting = false;

  std::string text = "GAME OVER";
  Vector2 txt_position = {213, 32};

  float start_spacing = 426;
  int spacing = start_spacing;

  menu_options options {
    OPT_RESTART,
    OPT_TITLE,
    OPT_QUIT
  };

  menu_options_txt options_text {
    "RESTART",
    "TITLE SCREEN",
    "QUIT GAME",
  };

  menu_options::iterator selected_option = options.begin();

  float spacing_percentage = 1.0;
  float spacing_time = 0.75;

  float zoom_percentage = 1.0; 
  float zoom_time = 4.0;

  float alpha_percentage = 1.0;
  float alpha_time = 2.0;

  float exit_time = 6.0;
  float exit_timestamp = 0.0;

  Texture background;
  Color bg_tint = WHITE;
  Rectangle bg_source;
  Rectangle bg_dest;
};
