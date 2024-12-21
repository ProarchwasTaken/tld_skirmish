// scenes/scene_index.cpp
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "utils_sound.h"
#include "utils_menu.h"
#include "scene_menu.h"
#include "scene_index.h"
#include <plog/Log.h>

constexpr float BOX_START_Y = 69;
constexpr float BOX_END_Y = 61;


IndexScene::IndexScene(Game &skirmish) : Scene(skirmish) {
  PLOGI << "Loaded Combatant Index scene.";
}

void IndexScene::updateScene() {
  menu_hud.update();

  if (ready == false) {
    transitionLerp();
  }
  else if (exiting) {
    skirmish->loadScene<MenuScene>(menu_hud);
  }
}

void IndexScene::transitionLerp() {
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

void IndexScene::checkInput() {  
  if (ready == false) {
    return;
  }

  bool key_down = IsKeyPressed(KEY_DOWN);
  bool key_up = IsKeyPressed(KEY_UP);
  bool key_x = IsKeyPressed(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_down = false; 
  bool btn_up = false;
  bool btn_b = false;

  if (gamepad_detected) {
    btn_down = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
  }

  if (key_down || btn_down) {
    Menu::nextOption(skir_options, selected_option, true);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(skir_options, selected_option, true);
  }
  else if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    ready = false;
    exiting = true;
  }
}

void IndexScene::drawCursor(Vector2 position) {
  float x = -8 + (cosf(CURRENT_TIME * 10) * 0.55);

  position = Vector2Add(position, {x, 3});
  DrawTextureV(*sprites::hud_menubox[3], position, WHITE);
}

void IndexScene::drawOptions() {
  int size = fonts::skirmish->baseSize;
  Vector2 position = {120, 68};

  for (int option : skir_options) {
    const char* text = TextFormat("%03i", option + 1);

    Color color;
    if (*selected_option == option) {
      color = COLORS::PALETTE[42];
      drawCursor(position);
    }
    else {
      color = WHITE;
    }

    DrawTextEx(*fonts::skirmish, text, position, size, 0, color);
    position.y += 11;
  }
}

void IndexScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();
  DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);

  DrawTexture(*sprites::hud_menubox[2], 109, box_y, box_tint);

  if (ready && exiting == false) {
    drawOptions();
  }
}
