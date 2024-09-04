// scenes/scene_subweapon.cpp
#include <raylib.h>
#include <cstdint>
#include <string>
#include <cmath>
#include <array>
#include "game.h"
#include "globals.h"
#include "utils.h"
#include "base/scene.h"
#include "scene_menu.h"
#include "scene_subweapon.h"
#include <plog/Log.h>

using std::string, std::array;


SubWeaponScene::SubWeaponScene(Game &skirmish) : Scene(skirmish) {
  icon_ids = {4, 5};

  weapon_options = {OPT_KNIFE, OPT_BALL};
  weapon_name = {"Birthright", "Cornerman"};
  selected_weapon = weapon_options.begin();

  confirm_options = {OPT_YES, OPT_NO};
  confirm_names = {"YES", "NO"};
  selected_confirm = confirm_options.begin();

  start_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Sub-Weapon Selection scene";
}

SubWeaponScene::~SubWeaponScene() {
  weapon_options.clear();
  weapon_name.clear();
  PLOGI << "Sub-Weapon Selection scene has been unloaded.";
}

void SubWeaponScene::checkInput() {
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
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
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
  else if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    skirmish->loadScene<MenuScene>();
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
      break;
    }
    case OPT_NO: {
      SoundUtils::play("opt_cancel");
      confirm = false;
    }
  }
}

void SubWeaponScene::drawWeaponIcons() {
  int icon_spacing = 24;
  Vector2 position = {195, 105};

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

  Vector2 position = {195 + offset, 95 + sinY};
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
  DrawTexture(*sprites::weapon_select[0], 0, 80, WHITE);
  DrawTexture(*sprites::weapon_select[1], 96, 144, WHITE);

  drawWeaponIcons();
  drawSelectionArrow();

  drawWeaponName();
  drawTechniqueData();

  if (confirm) {
    drawWarningText();
    drawConfirmOptions();
  }
}

