// scene/scene_settings.cpp
#include <raylib.h>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "utils_sound.h"
#include "scene_menu.h"
#include "scene_settings.h"
#include <plog/Log.h>


SettingsScene::SettingsScene(Game &skirmish) : Scene(skirmish) {
  PLOGI << "Loaded Settings scene.";
}

void SettingsScene::updateScene() {
  menu_hud.update();
}

void SettingsScene::checkInput() {
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

void SettingsScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();
  DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);

  DrawTexture(*sprites::hud_menubox[1], 109, 57, WHITE);
}
