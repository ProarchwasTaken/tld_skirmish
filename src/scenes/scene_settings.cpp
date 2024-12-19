// scene/scene_settings.cpp
#include <raylib.h>
#include <raymath.h>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "utils_sound.h"
#include "scene_menu.h"
#include "scene_settings.h"
#include <plog/Log.h>

constexpr float BOX_START_Y = 65;
constexpr float BOX_END_Y = 57;


SettingsScene::SettingsScene(Game &skirmish) : Scene(skirmish) {
  PLOGI << "Loaded Settings scene.";
}

void SettingsScene::updateScene() {
  menu_hud.update();

  if (ready == false) {
    transitionLerp();
  }
  else if (exiting) {
    skirmish->loadScene<MenuScene>(menu_hud);
  }
}

void SettingsScene::transitionLerp() {
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

void SettingsScene::checkInput() {
  if (ready == false) {
    return;
  }

  bool key_x = IsKeyReleased(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_b = false;

  if (gamepad_detected) {
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
  }

  if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    ready = false;
    exiting = true;
  }
}

void SettingsScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();
  DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);

  DrawTexture(*sprites::hud_menubox[1], 109, box_y, box_tint);
}
