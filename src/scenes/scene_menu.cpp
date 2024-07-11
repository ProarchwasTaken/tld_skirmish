// scenes/scene_menu.cpp
#include <raylib.h>
#include <string>
#include <functional>
#include <cstdint>
#include "globals.h"
#include "utils.h"
#include "scene_menu.h"
#include <plog/Log.h>

using std::function, std::string;


MenuScene::MenuScene(function<void(int)> load_scene) : Scene(load_scene) {
  PLOGV << "Loading MainMenu scene.";
  options = {
    OPT_PLAY,
    OPT_SETTINGS,
    OPT_CONTROLS,
    OPT_QUIT
  };
  options_text = {
    "PLAY GAME",
    "SETTINGS",
    "CONTROLS",
    "QUIT GAME"
  };

  selected_option = options.begin();
}

void MenuScene::checkInput() {
  bool key_down = IsKeyPressed(KEY_DOWN);
  bool key_up = IsKeyPressed(KEY_UP);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_down, btn_up = false;

  if (gamepad_detected) {
    btn_down = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
  }

  if (key_down || btn_down) {
    Menu::nextOption(options, selected_option, true);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(options, selected_option, true);
  }
}

void MenuScene::drawMenuOptions() {
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
    case OPT_QUIT: {
      text = "Exit the game and take a break.";
      break;
    }
    default: {
      text = "DESCRIPTION NOT FOUND!!!";
    }
  }

  Vector2 position = Text::alignCenter(fonts::skirmish, text, {213, 208},
                                       1, -3);
  DrawTextEx(*fonts::skirmish, text.c_str(), position, size, -3, 
             COLORS::PALETTE[2]);
}

void MenuScene::drawScene() {
  drawMenuOptions();
  drawOptionDescription();
}
