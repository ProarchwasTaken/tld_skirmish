// scenes/scene_splash.cpp
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/scene.h"
#include "utils_text.h"
#include "game.h"
#include "utils_music.h"
#include "scene_title.h"
#include "scene_splash.h"
#include <plog/Log.h>


SplashScene::SplashScene(Game &skirmish) : Scene(skirmish) {
  icon = LoadTextureFromImage(GAME_ICON);

  txt_position = Text::alignCenter(fonts::skirmish, text, txt_position, 
                                   1, -3);

  MusicUtils::play(0);
  start_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Splash Screen.";
}

SplashScene::~SplashScene() {
  UnloadTexture(icon);
}

void SplashScene::updateScene() {
  float time_elapsed = CURRENT_TIME - start_timestamp;

  if (time_elapsed >= start_time) {
    skirmish->loadScene<TitleScene>();
  }
  else {
    splashSequence(time_elapsed);
    bg_transition.interpolate();
  }
}

void SplashScene::splashSequence(float time_elapsed) {
  if (time_elapsed >= 1.0) {
    interpolateAlpha();
  } 

  if (time_elapsed >= 3.0) {
    interpolateSize();
  }

  if (bg_fading_out == false && time_elapsed >= 6.0) {
    bg_transition.fadeout(2, BLACK);
    bg_fading_out = true;
  }
  
  if (draw_text && time_elapsed >= 9.5) {
    draw_text = false;
  }
}

void SplashScene::interpolateAlpha() {
  if (alpha_percentage == 1.0) {
    return;
  }

  alpha_percentage += GetFrameTime() * start_time;
  alpha_percentage = Clamp(alpha_percentage, 0.0, 1.0);

  icon_tint.a = Lerp(0, 255, alpha_percentage);
}

void SplashScene::interpolateSize() {
  if (size_percentage == 1.0) {
    return;
  }

  size_percentage += GetFrameTime() * size_time;
  size_percentage = Clamp(size_percentage, 0.0, 1.0);

  line_size = Lerp(0, max_line_size, size_percentage);
}

void SplashScene::checkInput() {
  int inputs = GetKeyPressed() + GetGamepadButtonPressed();
  if (inputs != 0 && IsWindowFocused()) {
    skirmish->loadScene<TitleScene>();
  }
}

void SplashScene::drawScene() {
  if (alpha_percentage != 0.0) {
    DrawTextureV(icon, icon_position, icon_tint);
  }

  bg_transition.draw();

  if (size_percentage != 0.0) {
    DrawLineEx({0, 120}, {426, 120}, line_size, BLACK);
  }

  if (draw_text && line_size == max_line_size) {
    int size = fonts::skirmish->baseSize;
    DrawTextEx(*fonts::skirmish, text.c_str(), txt_position, size, -3, 
               COLORS::PALETTE[42]);
  }
}
