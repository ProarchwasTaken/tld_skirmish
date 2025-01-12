// scenes/scene_subweapon.cpp
#include <raylib.h>
#include <raymath.h>
#include <cstdint>
#include <string>
#include <cmath>
#include <array>
#include <triblib/base/scene.h>
#include "game.h"
#include "globals.h"
#include "utils_text.h"
#include "utils_menu.h"
#include "utils_sound.h"
#include "utils_music.h"
#include "scene_menu.h"
#include "scene_pregame.h"
#include "scene_subweapon.h"
#include <plog/Log.h>

using std::string, std::array;
constexpr Rectangle default_frame = {0, 108, 426, 56};
constexpr Rectangle frame_source = {0, 0, 426, 56};


SubWeaponScene::SubWeaponScene(Game &skirmish, bool from_gameover) : 
  Scene(skirmish) 
{
  frame_dest = default_frame;
  frame_dest.height = 0;
  this->from_gameover = from_gameover;

  start_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Sub-Weapon Selection scene";
}

SubWeaponScene::~SubWeaponScene() {
  weapon_options.clear();
  weapon_name.clear();
  PLOGI << "Sub-Weapon Selection scene has been unloaded.";
}

void SubWeaponScene::updateScene() {
  menu_hud.update();

  if (ready) {
    return;
  }

  if (going_back) {
    ready_percentage -= GetFrameTime() / ready_time;
  }
  else {
    ready_percentage += GetFrameTime() / ready_time;
  }
  ready_percentage = Clamp(ready_percentage, 0.0, 1.0);

  frameInterpolation();
  windowInterpolation();

  float time_elapsed = CURRENT_TIME - start_timestamp;
  if (time_elapsed < ready_time) {
    return;
  }
  
  if (going_back) {
    skirmish->loadScene<MenuScene>(menu_hud);
  }
  else {
    ready = true;
  }
}

void SubWeaponScene::frameInterpolation() {
  float max_height = default_frame.height;
  frame_dest.height = Lerp(0, max_height, ready_percentage);
}

void SubWeaponScene::windowInterpolation() {
  float end_window_y = 144;
  float percentage = 1.0 - ready_percentage;

  window_position.y = Lerp(end_window_y, 240, percentage);
}

void SubWeaponScene::checkInput() {
  if (ready == false) {
    return;
  }

  bool key_right = IsKeyPressed(KEY_RIGHT);
  bool key_left = IsKeyPressed(KEY_LEFT);
  bool key_z = IsKeyPressed(KEY_Z);
  bool key_x = IsKeyPressed(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_right = false;
  bool btn_left = false;
  bool btn_a = false;
  bool btn_b = false;

  if (gamepad_detected) {
    btn_right = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    btn_left = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    btn_a = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
  }

  if (confirm) {
    checkConfirmInput({key_right, key_left, key_z, key_x},
                      {btn_right, btn_left, btn_a, btn_b});
    return;
  }

  if (key_right || btn_right) {
    Menu::nextOption(weapon_options, selected_weapon, true);
  }
  else if (key_left || btn_left) {
    Menu::previousOption(weapon_options, selected_weapon, true);
  }
  else if (key_z || btn_a) {
    SoundUtils::play("opt_confirm");
    confirm = true;
  }
  else if (from_gameover == false && (key_x || btn_b)) {
    SoundUtils::play("opt_cancel");
    ready = false;
    going_back = true;

    start_timestamp = CURRENT_TIME;
  }
}

void SubWeaponScene::checkConfirmInput(array<bool, 4> key_input,
                                       array<bool, 4> btn_input) 
{
  if (key_input[0] || btn_input[0]) {
    Menu::nextOption(confirm_options, selected_confirm, true);
  }
  else if (key_input[1] || btn_input[1]) {
    Menu::previousOption(confirm_options, selected_confirm, true);
  }
  else if (key_input[2] || btn_input[2]) {
    selectConfirmOption();
  }
  else if (key_input[3] || btn_input[3]) {
    SoundUtils::play("opt_cancel");
    confirm = false;
  }
}

void SubWeaponScene::selectConfirmOption() {
  switch (*selected_confirm) {
    case OPT_YES: {
      SoundUtils::play("opt_confirm");
      MusicUtils::fadeout(0.0, 6.0);
      
      skirmish->loadScene<PregameScene>(*selected_weapon);
      break;
    }
    case OPT_NO: {
      SoundUtils::play("opt_cancel");
      confirm = false;
      break;
    }
  }
}

void SubWeaponScene::drawWeaponIcons() {
  int icon_spacing = 24;
  Vector2 position = {179, 105};

  for (int id : icon_ids) {
    DrawTextureV(*sprites::weapon_select[id], position, WHITE);
    position.x += icon_spacing;
  }
}

void SubWeaponScene::drawSelectionArrow() {
  float offset = 24 * *selected_weapon;
  float sinY = 0;

  float time_elapsed = CURRENT_TIME - start_timestamp;
  if (confirm != true) {
    sinY = sinf(time_elapsed * 2.5) * 0.75;
  }

  Vector2 position = {179 + offset, 95 + sinY};
  DrawTextureV(*sprites::weapon_select[2], position, WHITE);
}

void SubWeaponScene::drawWeaponName() {
  int size = fonts::skirmish->baseSize;
  int index = *selected_weapon;

  string *name = &weapon_name[index];
  Vector2 position = Text::alignCenter(fonts::skirmish, *name, 
                                       {213, 124}, 1, -1);

  DrawTextEx(*fonts::skirmish, name->c_str(), position, size, -1, 
             COLORS::PALETTE[42]);
}

void SubWeaponScene::drawTechniqueData() {
  int index = *selected_weapon;

  SubWeaponData *data = &weapon_data[index];

  Vector2 base_position = {100, 153};
  drawTechLine(data->tech_light, base_position);

  base_position.y += 23;
  drawTechLine(data->tech_heavy, base_position);
}

void SubWeaponScene::drawTechLine(TechData &tech, Vector2 position) {
  int size = fonts::skirmish->baseSize;

  DrawTextEx(*fonts::skirmish, tech.name, position, size, -3, 
             COLORS::PALETTE[42]);

  position.x = 180;
  DrawTextEx(*fonts::skirmish, TextFormat("%i MP", tech.mp_cost), 
             position, size, -3, COLORS::PALETTE[42]);

  position.x = 229;
  DrawTextEx(*fonts::skirmish, tech.description, position, size, -3, 
             COLORS::PALETTE[42]);
}

void SubWeaponScene::drawWarningText() {
  int size = fonts::skirmish->baseSize;
  string text = "IS THIS REALLY WHAT YOU NEED?";

  Vector2 position = Text::alignCenter(fonts::skirmish, text, {213, 210}, 
                                       1, -3);
  DrawTextEx(*fonts::skirmish, text.c_str(), position, size, -3, 
             COLORS::PALETTE[32]);
}

void SubWeaponScene::drawConfirmOptions() {
  int size = fonts::skirmish->baseSize;
  Vector2 position = {176, 222};

  for (uint8_t option : confirm_options) {
    string *text = &confirm_names[option];

    Color color;
    if (*selected_confirm == option) {
      color = COLORS::PALETTE[22];
    }
    else {
      color = WHITE;
    }

    DrawTextEx(*fonts::skirmish, text->c_str(), position, size, -3, 
               color);
    position.x += 54;
  }
}

void SubWeaponScene::drawScene() {
  if (from_gameover == false) {
    DrawTexture(skirmish->bg_main, 0, 0, WHITE);
    menu_hud.draw();
    DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);
  }

  float frame_originY = frame_dest.height / 2;
  DrawTexturePro(*sprites::weapon_select[0], frame_source, frame_dest,
                 {0, frame_originY}, 0, WHITE);
  DrawTextureV(*sprites::weapon_select[1], window_position, WHITE);

  if (ready == false) {
    return;
  }

  drawWeaponIcons();
  drawSelectionArrow();

  drawWeaponName();
  drawTechniqueData();

  if (confirm) {
    drawWarningText();
    drawConfirmOptions();
  }
}

