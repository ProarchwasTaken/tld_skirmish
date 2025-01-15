// hud/hud_pause.cpp
#include <raylib.h>
#include "globals.h"
#include "scene_gameplay.h"
#include "scene_title.h"
#include "utils_menu.h"
#include <triblib/utils/sound.h>
#include "hud_menu.h"
#include "hud_pause.h"
#include <plog/Log.h>


PauseHud::PauseHud(GameplayScene *scene) : MenuHud(true) { 
  shift_gradient = false;
  this->scene = scene;
}

void PauseHud::restartOpening() {
  MenuHud::restartOpening();
  fadeInSpade();
}

void PauseHud::update() {
  MenuHud::update();
  menu_btns.update();

  if (closing_menu && spade_state == SPADE_STANDBY) {
    closing_menu = false;
    selectOption();
  }
}

void PauseHud::selectOption() {
  switch (*selected_option) {
    case OPT_RESUME: {
      scene->resumeGame();
      break;
    }
    case OPT_TITLE: {
      scene->skirmish->loadScene<TitleScene>(true);
      break;
    }
    case OPT_QUIT: {
      PLOGV << "Setting 'EXIT_GAME' to true.";
      EXIT_GAME = true;
      break;
    }
  }
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
  else if (key_z || btn_a) {
    SoundUtils::play("opt_confirm");
    fadeOutSpade();
    closing_menu = true;
  }
}

void PauseHud::draw() {
  MenuHud::draw();

  if (opening == false) {
    DrawTexture(*sprites::hud_mainmenu[8], 4, 21, WHITE);
    menu_btns.drawMenuButtons();
  }
}

