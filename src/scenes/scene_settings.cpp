// scene/scene_settings.cpp
#include <cstdint>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <triblib/base/scene.h>
#include "globals.h"
#include "game.h"
#include "utils_sound.h"
#include "utils_text.h"
#include "utils_menu.h"
#include "utils_settings.h"
#include "scene_menu.h"
#include "scene_settings.h"
#include <plog/Log.h>

using std::string, std::to_string;
constexpr float BOX_START_Y = 65;
constexpr float BOX_END_Y = 57;


SettingsScene::SettingsScene(Game &skirmish) : Scene(skirmish) {
  sfx_volume = settings::sfx_volume * 100;
  bgm_volume = settings::bgm_volume * 100;
  framerate = settings::framerate;
  fullscreen = settings::fullscreen;
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

  bool key_right = IsKeyPressed(KEY_RIGHT);
  bool key_left = IsKeyPressed(KEY_LEFT);
  bool key_down = IsKeyPressed(KEY_DOWN);
  bool key_up = IsKeyPressed(KEY_UP);
  bool key_z = IsKeyPressed(KEY_Z);
  bool key_x = IsKeyPressed(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_right = false;
  bool btn_left = false;
  bool btn_down = false; 
  bool btn_up = false;
  bool btn_a = false;
  bool btn_b = false;

  if (gamepad_detected) {
    btn_right = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    btn_left = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    btn_down = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    btn_a = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
  }

  if (key_left || btn_left) {
    decrementSetting();
  }
  else if (key_right || btn_right) {
    incrementSetting();
  }
  else if (key_down || btn_down) {
    Menu::nextOption(options, selected_option, false);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(options, selected_option, false);
  }
  else if (*selected_option == OPT_APPLY && (key_z || btn_a)) {
    SoundUtils::play("opt_confirm");
    applyOptionSelected();
  }
  else if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    ready = false;
    exiting = true;
  }
}

void SettingsScene::incrementSetting() {
  int *setting_value;
  int increment;
  int max_value;

  switch (*selected_option) {
    case OPT_SOUND: {
      setting_value = &sfx_volume;
      increment = 10;
      max_value = 100;
      break;
    }
    case OPT_MUSIC: {
      setting_value = &bgm_volume;
      increment = 10;
      max_value = 100;
      break;
    }
    case OPT_FRAMERATE: {
      setting_value = &framerate;
      increment = 5;
      max_value = 300;
      break;
    }
    case OPT_FULLSCREEN: {
      SoundUtils::play("opt_change");
      fullscreen = !fullscreen;
      return;
    }
    default: {
      return;
    }
  }

  *setting_value += increment;
  if (*setting_value <= max_value) {
    SoundUtils::play("opt_change");
  }
  else {
    *setting_value = max_value;
    SoundUtils::play("opt_error");
  }
}

void SettingsScene::decrementSetting() {
  int *setting_value;
  int decrement;
  int min_value;

  switch (*selected_option) {
    case OPT_SOUND: {
      setting_value = &sfx_volume;
      decrement = 10;
      min_value = 0;
      break;
    }
    case OPT_MUSIC: {
      setting_value = &bgm_volume;
      decrement = 10;
      min_value = 0;
      break;
    }
    case OPT_FRAMERATE: {
      setting_value = &framerate;
      decrement = 5;
      min_value = 30;
      break;
    }
    case OPT_FULLSCREEN: {
      SoundUtils::play("opt_change");
      fullscreen = !fullscreen;
      return;
    }
    default: {
      return;
    }
  }

  *setting_value -= decrement;
  if (*setting_value >= min_value) {
    SoundUtils::play("opt_change");
  }
  else {
    *setting_value = min_value;
    SoundUtils::play("opt_error");
  }
}

void SettingsScene::applyOptionSelected() {
  PLOGI << "Applying settings.";
  Settings::apply(sfx_volume / 100.0, bgm_volume / 100.0, framerate, 
                  fullscreen);
  skirmish->fullscreenCheck();
}

void SettingsScene::drawOption(uint8_t option, float current_y) {
  int txt_size = fonts::skirmish->baseSize;
  int spacing = 0;
  string text;

  switch (option) {
    case OPT_SOUND: {
      text = to_string(sfx_volume);
      break;
    }
    case OPT_MUSIC: {
      text = to_string(bgm_volume);
      break;
    }
    case OPT_FRAMERATE:  {
      text = to_string(framerate);
      break;
    }
    case OPT_FULLSCREEN: {
      if (fullscreen) text = "TRUE";
      else text = "FALSE";
      break;
    }
    case OPT_APPLY: {
      text = "APPLY SETTINGS";
      spacing = -3;
      current_y = 164;
      break;
    }
  }

  Color txt_color;
  if (option == *selected_option) {
    txt_color = COLORS::PALETTE[22];
  }
  else {
    txt_color = WHITE;
  }

  Vector2 position = Text::alignCenter(fonts::skirmish, text, 
                                       {213, current_y}, 1, spacing);
  DrawTextEx(*fonts::skirmish, text.c_str(), position, txt_size, spacing, 
             txt_color);
}

void SettingsScene::drawMenuOptions() {
  float current_y = 76;

  for (uint8_t option : options) {
    drawOption(option, current_y);
    current_y += 24;
  }
}

void SettingsScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();
  DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);

  DrawTexture(*sprites::hud_menubox[1], 109, box_y, box_tint);

  if (ready && exiting == false) {
    drawMenuOptions();
  }
}
