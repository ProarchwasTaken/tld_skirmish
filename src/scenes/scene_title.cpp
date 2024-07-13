// scenes/scene_title.cpp
#include <raylib.h>
#include <functional>
#include <string>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "utils.h"
#include "scene_title.h"
#include <plog/Log.h>

using std::function, std::string;


TitleScene::TitleScene(function<void(int)> load_scene) : Scene(load_scene) 
{
  PLOGI << "Loading Title scene.";
  setupTitle();
  setupCopyright();
  setupEnter();

  blink_interval = 0.5;
  PLOGI << "Title scene has been successfully loaded.";
}

TitleScene::~TitleScene() {
  PLOGI << "Unloading title scene";
}

void TitleScene::setupTitle() {
  txt_title = "True Human Tribulation II";

  title_position = Text::alignCenter(fonts::skirmish, txt_title, 
                                     {213, 32}, 2, -3);
}

void TitleScene::setupCopyright() {
  txt_copyright = "@2024 Tyler Dillard";

  cpr_position = Text::alignRight(fonts::skirmish, txt_copyright.c_str(), 
                                  {CANVAS_WIDTH, 228}, 1, -3);
}

void TitleScene::setupEnter() {
  txt_enter = "PRESS ENTER";
  enter_position = Text::alignCenter(fonts::skirmish, txt_enter.c_str(), 
                                     {213, 160}, 1, 0);
}

void TitleScene::checkInput() {
  bool key_enter = IsKeyPressed(KEY_ENTER);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_start = false;

  if (gamepad_detected) {
    btn_start = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
  }

  if (key_enter || btn_start) {
    load_scene(SCENE_MENU);
    return;
  }
}

void TitleScene::updateScene(double &delta_time) {
  float time_elapsed = GetTime() - blink_timestamp;

  if (time_elapsed >= blink_interval) {
    draw_enter = !draw_enter;
    blink_timestamp = GetTime();
  }
}

void TitleScene::drawScene() {
  int size = fonts::skirmish->baseSize;
  
  DrawTextEx(*fonts::skirmish, txt_title.c_str(), title_position, 
             size * 2, -3, COLORS::PALETTE[42]);
  DrawTextEx(*fonts::skirmish, txt_copyright.c_str(), cpr_position, size,
             -3, WHITE);

  if (draw_enter) {
    DrawTextEx(*fonts::skirmish, txt_enter.c_str(), enter_position, size, 
               0, WHITE);
  }
}
