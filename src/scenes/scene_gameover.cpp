// scene/scene_gameover.cpp
#include <raylib.h>
#include <raymath.h>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "utils_text.h"
#include "scene_gameover.h"


GameOverScene::GameOverScene(Game &skirmish, Texture background) : 
  Scene(skirmish) 
{
  txt_position = Text::alignCenter(fonts::skirmish, text, txt_position, 
                                   2, start_spacing);
  this->background = background;
}

GameOverScene::~GameOverScene() {
  UnloadTexture(background);
}

void GameOverScene::updateScene() {
  if (ready == false) {
    interpolateSpacing();
  }
}

void GameOverScene::interpolateSpacing() {
  percentage -= GetFrameTime() / lerp_time;
  percentage = Clamp(percentage, 0, 1.0);

  spacing = Lerp(0, start_spacing, percentage);
  txt_position = Text::alignCenter(fonts::skirmish, text, txt_position, 
                                   2, spacing);

  if (percentage == 0.0) {
    ready = true;
  }
}

void GameOverScene::drawScene() {
  int gm_size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, text.c_str(), txt_position, gm_size, 
             spacing, WHITE);
}
