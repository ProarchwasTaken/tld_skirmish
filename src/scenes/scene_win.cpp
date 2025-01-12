// scenes/scene_win.cpp
#include <raylib.h>
#include <raymath.h>
#include <triblib/base/scene.h>
#include "globals.h"
#include "game.h"
#include "utils_text.h"
#include "scene_title.h"
#include "scene_win.h"
#include <plog/Log.h>

WinScene::WinScene(Game &skirmish) : Scene(skirmish) {
  position = Text::alignCenter(fonts::skirmish, text, position, 2, -1);
  txt_color = COLORS::PALETTE[14];

  scene_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Win scene.";
}

void WinScene::updateScene() {
  float time_elapsed = CURRENT_TIME - scene_timestamp;

  if (time_elapsed >= scene_time / 2) {
    interpolateAlpha();
  }

  if (time_elapsed >= scene_time) {
    skirmish->loadScene<TitleScene>(true);
  }
}

void WinScene::interpolateAlpha() {
  if (percentage == 0.0) {
    return;
  }

  percentage -= GetFrameTime() / fade_time;
  percentage = Clamp(percentage, 0.0, 1.0);

  txt_color.a = Lerp(0, 255, percentage);
}

void WinScene::drawScene() {
  int size = fonts::skirmish->baseSize * 2;
  DrawTextEx(*fonts::skirmish, text.c_str(), position, size, -1, 
             txt_color);
}
