// scene/scene_gameover.cpp
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <triblib/base/scene.h>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include <triblib/utils/text.h>
#include <triblib/utils/sound.h>
#include <triblib/utils/music.h>
#include "utils/menu.h"
#include "scenes/scene_title.h"
#include "scenes/scene_subweapon.h"
#include "scenes/scene_gameover.h"
#include <plog/Log.h>

using std::string;
constexpr float HALF_CANVAS_W = CANVAS_WIDTH / 2;
constexpr float HALF_CANVAS_H = CANVAS_HEIGHT / 2;


GameOverScene::GameOverScene(Game &skirmish, Image screenshot): 
  Scene(skirmish) 
{
  txt_position = Text::alignCenter(fonts::skirmish, text, txt_position, 
                                   2, start_spacing);

  setupBackground(screenshot);
  UnloadImage(screenshot);

  SoundUtils::play("game_over");
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
  bg_dest = {213, 120, CANVAS_WIDTH, CANVAS_HEIGHT};
}

void GameOverScene::updateScene() {
  if (ready) {
    return;
  }

  if (exiting == false) {
    interpolateSpacing();
  }
  else {
    exitSequence();
  }
}

void GameOverScene::exitSequence() {
  interpolateZoom();

  if (zoom_percentage <= 0.5) {
    interpolateAlpha();
  }

  float time_elapsed = CURRENT_TIME - exit_timestamp;
  if (time_elapsed >= exit_time) {
    PLOGI << "Ending exit sequence.";
    endExitSequence();
  }
}

void GameOverScene::endExitSequence() {
  switch (*selected_option) {
    case OPT_TITLE: {
      skirmish->loadScene<TitleScene>(true);
      break;
    }
    case OPT_QUIT: {
      PLOGV << "Setting 'EXIT_GAME' to true.";
      EXIT_GAME = true;
      break;
    }
  }
}

void GameOverScene::interpolateSpacing() {
  spacing_percentage -= GetFrameTime() / spacing_time;
  spacing_percentage = Clamp(spacing_percentage, 0, 1.0);

  spacing = Lerp(0, start_spacing, spacing_percentage);
  txt_position = Text::alignCenter(fonts::skirmish, text, {213, 32}, 2, 
                                   spacing);

  if (spacing_percentage == 0.0) {
    PLOGI << "Text interpolation complete.";
    MusicUtils::play(2);
    ready = true;
  }
}

void GameOverScene::interpolateZoom() {
  if (zoom_percentage == 0.0) {
    return;
  }

  zoom_percentage -= GetFrameTime() / zoom_time;
  zoom_percentage = Clamp(zoom_percentage, 0.0, 1.0);

  bg_dest.width = Lerp(HALF_CANVAS_W, CANVAS_WIDTH, zoom_percentage);
  bg_dest.height = Lerp(HALF_CANVAS_H, CANVAS_HEIGHT, zoom_percentage);
}

void GameOverScene::interpolateAlpha() {
  if (alpha_percentage == 0.0) {
    return;
  }

  alpha_percentage -= GetFrameTime() / alpha_time;
  alpha_percentage = Clamp(alpha_percentage, 0.0, 1.0);

  bg_tint.a = Lerp(0, 255, alpha_percentage);
}

void GameOverScene::checkInput() {
  if (ready == false) {
    return;
  }

  bool key_down = IsKeyPressed(KEY_DOWN);
  bool key_up = IsKeyPressed(KEY_UP);
  bool key_z = IsKeyPressed(KEY_Z);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_down = false; 
  bool btn_up = false;
  bool btn_a = false;

  if (gamepad_detected) {
    btn_down = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    btn_a = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
  }

  if (key_down || btn_down) {
    Menu::nextOption(options, selected_option, true);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(options, selected_option, true);
  }
  else if (key_z || btn_a) {
    SoundUtils::play("opt_confirm");
    selectOption();
  }
}

void GameOverScene::selectOption() {
  switch (*selected_option) {
    case OPT_RESTART: {
      skirmish->loadScene<SubWeaponScene>(true);
      break;
    }
    default: {
      ready = false;
      exiting = true;

      PLOGI << "Beginning exit sequence.";
      MusicUtils::fadeout(0.0, 5);
      exit_timestamp = CURRENT_TIME;
    }
  }
}

void GameOverScene::drawMenuOptions() {
  int size = fonts::skirmish->baseSize;
  float current_y = 90;

  for (uint8_t option : options) {
    string *text = &options_text[option];
    Vector2 position = Text::alignCenter(fonts::skirmish, *text, 
                                         {213, current_y}, 1, 0);

    Color color;
    if (*selected_option == option) {
      color = COLORS::PALETTE[22];
    }
    else {
      color = WHITE;
    }

    DrawTextEx(*fonts::skirmish, text->c_str(), position, size, 0, color);
    current_y += size;
  }
}

void GameOverScene::drawScene() {
  Vector2 bg_origin = {bg_dest.width / 2, bg_dest.height / 2};
  DrawTexturePro(background, bg_source, bg_dest, bg_origin, 0, bg_tint);

  if (exiting) {
    return;
  }

  int gm_size = fonts::skirmish->baseSize * 2;
  DrawTextEx(*fonts::skirmish, text.c_str(), txt_position, gm_size, 
             spacing, COLORS::PALETTE[32]);

  if (ready) {
    drawMenuOptions();
  }
}
