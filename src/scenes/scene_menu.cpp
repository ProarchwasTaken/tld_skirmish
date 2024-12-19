// scenes/scene_menu.cpp
#include <raylib.h>
#include <string>
#include "base/scene.h"
#include "game.h"
#include "globals.h"
#include "utils_text.h"
#include "utils_menu.h"
#include "utils_sound.h"
#include "hud_menu.h"
#include "scene_subweapon.h"
#include "scene_settings.h"
#include "scene_control.h"
#include "scene_menu.h"
#include <plog/Log.h>

using std::string;


MenuScene::MenuScene(Game &skirmish) : Scene(skirmish) {
  menu_hud.fadeInSpade();
  PLOGI << "Loaded MainMenu scene.";
}

MenuScene::MenuScene(Game &skirmish, MenuHud &menu_hud) : Scene(skirmish) 
{
  this->menu_hud = menu_hud;
  this->menu_hud.fadeInSpade();
  PLOGI << "Loaded MainMenu scene.";
}

MenuScene::~MenuScene() {
  options.clear();
  options_text.clear();
  PLOGI << "Successfully unloaded MainMenu scene.";
}

void MenuScene::updateScene() {
  menu_hud.update();
  menu_btns.update();

  if (exiting_scene && menu_hud.spade_state == SPADE_STANDBY) {
    selectOption();
  }
}

void MenuScene::checkInput() {
  if (menu_hud.opening || menu_hud.spade_state != SPADE_STANDBY) {
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
    menu_hud.fadeOutSpade();
    exiting_scene = true;
  }
}

void MenuScene::selectOption() {
  switch (*selected_option) {
    case OPT_PLAY: {
      skirmish->loadScene<SubWeaponScene>(false);
      break;
    }
    case OPT_SETTINGS: {
      skirmish->loadScene<SettingsScene>();
      break;
    }
    case OPT_CONTROLS: {
      skirmish->loadScene<ControlScene>();
      break;
    }
    case OPT_QUIT: {
      PLOGV << "Setting 'EXIT_GAME' to true.";
      EXIT_GAME = true;
      break;
    }
    default: {
      SoundUtils::play("opt_error");
      exiting_scene = false;
      PLOGI << "Menu option not fully implemented yet!";
    } 
  }
}

void MenuScene::drawOptionDescription() {
  int size = fonts::skirmish->baseSize;
  string text;

  switch (*selected_option) {
    case OPT_PLAY: {
      text = "Begin the game.";
      break;
    }
    case OPT_SETTINGS: {
      text = "Tweak the game's settings to your liking.";
      break;
    }
    case OPT_CONTROLS: {
      text = "Educate yourself on the game's controls.";
      break;
    }
    case OPT_INDEX: {
      text = "The Combatant Index. Written by an third-party.";
      break;
    }
    case OPT_QUIT: {
      text = "Close the game and take a break.";
      break;
    }
    default: {
      text = "DESCRIPTION NOT FOUND!!!";
    }
  }

  Vector2 position = Text::alignRight(fonts::skirmish, text, {418, 20},
                                      1, -3);
  DrawTextEx(*fonts::skirmish, text.c_str(), position, size, -3, 
             COLORS::PALETTE[2]);
}

void MenuScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();

  if (menu_hud.opening == false) {
    DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);
    menu_btns.drawMenuButtons();
    drawOptionDescription();
  }
}
