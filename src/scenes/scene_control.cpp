// scenes/scene_control.cpp
#include <raylib.h>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "utils_sound.h"
#include "scene_menu.h"
#include "scene_control.h"
#include <plog/Log.h>


ControlScene::ControlScene(Game &skirmish) : Scene(skirmish) {
  PLOGI << "Loaded Controls scene.";
}

void ControlScene::checkInput() {
  bool key_x = IsKeyReleased(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_b = false;

  if (gamepad_detected) {
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
  }

  if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    skirmish->loadScene<MenuScene>(menu_hud);
  }
}

void ControlScene::drawScene() {
  menu_hud.draw();
  DrawTexture(*sprites::hud_menubox[0], 81, 56, WHITE);
}
