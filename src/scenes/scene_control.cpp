// scenes/scene_control.cpp
#include <raylib.h>
#include <raymath.h>
#include <triblib/base/scene.h>
#include "globals.h"
#include "game.h"
#include <triblib/utils/sound.h>
#include "scenes/scene_menu.h"
#include "scenes/scene_control.h"
#include <plog/Log.h>

constexpr float BOX_START_Y = 64;
constexpr float BOX_END_Y = 56;


ControlScene::ControlScene(Game &skirmish) : Scene(skirmish) {
  PLOGI << "Loaded Controls scene.";
}

void ControlScene::updateScene() {
  menu_hud.update();

  if (ready == false) {
    transitionLerp();
  }
  else if (exiting) { 
    skirmish->loadScene<MenuScene>(menu_hud);
  }
}

void ControlScene::transitionLerp() {
  if (exiting == false) {
    ready_percentage += GetFrameTime() / ready_time;
  }
  else {
    ready_percentage -= GetFrameTime() / ready_time; 
  }

  ready_percentage = Clamp(ready_percentage, 0.0, 1.0);

  box_y = Lerp(BOX_END_Y, BOX_START_Y, 1.0 - ready_percentage);
  box_tint.a = Lerp(0, 255, ready_percentage);

  if (ready_percentage == 0.0 || ready_percentage == 1.0) {
    ready = true;
  }
}

void ControlScene::checkInput() {
  if (ready == false) {
    return;
  }

  bool key_x = IsKeyPressed(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_b = false;

  if (gamepad_detected) {
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
  }

  if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    ready = false;
    exiting = true;
  }
}

void ControlScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();
  DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);

  DrawTexture(*sprites::hud_menubox[0], 81, box_y, box_tint);
}
