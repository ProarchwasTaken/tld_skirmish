// hud/hud_pause.cpp
#include <raylib.h>
#include "scene_gameplay.h"
#include "utils_menu.h"
#include "utils_sound.h"
#include "hud_menu.h"
#include "hud_pause.h"


PauseHud::PauseHud(GameplayScene *scene) : MenuHud(true) { 
  shift_gradient = false;
  this->scene = scene;
}

void PauseHud::restartOpening() {
  MenuHud::restartOpening();
  fadeInSpade();
}

void PauseHud::checkInput() {
  if (opening && spade_state != SPADE_STANDBY) {
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
    Menu::nextOption(options, selected_option, false);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(options, selected_option, false);
  }
}

void PauseHud::update() {
  MenuHud::update();
  menu_btns.update();
}

void PauseHud::draw() {
  MenuHud::draw();

  if (opening == false) {
    DrawTexture(*sprites::hud_mainmenu[8], 4, 21, WHITE);
    menu_btns.drawMenuButtons();
  }
}

