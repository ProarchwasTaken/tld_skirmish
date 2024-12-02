// scene/scene_gameover.cpp
#include <raylib.h>
#include <raymath.h>
#include "base/scene.h"
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "utils_text.h"
#include "scene_gameover.h"
#include <plog/Log.h>


GameOverScene::GameOverScene(Game &skirmish, Image screenshot): 
  Scene(skirmish) 
{
  txt_position = Text::alignCenter(fonts::skirmish, text, txt_position, 
                                   2, start_spacing);

  setupBackground(screenshot);
  UnloadImage(screenshot);
  PLOGI << "Loaded Game Over scene.";
}

GameOverScene::~GameOverScene() {
  UnloadTexture(background);
}

void GameOverScene::setupBackground(Image &screenshot) {
  background = LoadTextureFromImage(screenshot);
  float width = GetScreenWidth();
  float height = GetScreenHeight();

  bg_source = {0, 0, width, height};
  bg_dest = {0, 0, CANVAS_WIDTH, CANVAS_HEIGHT};
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
  txt_position = Text::alignCenter(fonts::skirmish, text, {213, 32}, 2, 
                                   spacing);

  if (percentage == 0.0) {
    PLOGI << "Text interpolation complete.";
    ready = true;
  }
}

void GameOverScene::drawScene() {
  DrawTexturePro(background, bg_source, bg_dest, {0, 0}, 0, WHITE);

  int gm_size = fonts::skirmish->baseSize * 2;
  DrawTextEx(*fonts::skirmish, text.c_str(), txt_position, gm_size, 
             spacing, COLORS::PALETTE[32]);
}
