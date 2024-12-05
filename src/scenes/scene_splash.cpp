// scenes/scene_splash.cpp
#include <raylib.h>
#include "globals.h"
#include "base/scene.h"
#include "utils_text.h"
#include "game.h"
#include "scene_title.h"
#include "scene_splash.h"


SplashScene::SplashScene(Game &skirmish) : Scene(skirmish) {
  icon = LoadTextureFromImage(GAME_ICON);

  txt_position = Text::alignCenter(fonts::skirmish, text, txt_position, 
                                   1, -3);

  start_timestamp = CURRENT_TIME;
}

SplashScene::~SplashScene() {
  UnloadTexture(icon);
}

void SplashScene::updateScene() {
  float time_elapsed = CURRENT_TIME - start_timestamp;
  if (time_elapsed >= start_time) {
    skirmish->loadScene<TitleScene>();
  }
}

void SplashScene::checkInput() {
  if (GetKeyPressed() + GetGamepadButtonPressed() > 0) {
    skirmish->loadScene<TitleScene>();
  }
}

void SplashScene::drawScene() {

}
