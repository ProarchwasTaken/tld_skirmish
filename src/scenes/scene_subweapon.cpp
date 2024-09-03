// scenes/scene_subweapon.cpp
#include <raylib.h>
#include <string>
#include <cmath>
#include "game.h"
#include "globals.h"
#include "utils.h"
#include "base/scene.h"
#include "scene_subweapon.h"
#include <plog/Log.h>

using std::string;


SubWeaponScene::SubWeaponScene(Game &skirmish) : Scene(skirmish) {
  icon_ids = {4, 5};

  options = {
    OPT_KNIFE,
    OPT_BALL
  };

  option_name = {
    "Knife",
    "Basket Ball"
  };

  selected_option = options.begin();
  start_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Sub-Weapon Selection scene";
}

SubWeaponScene::~SubWeaponScene() {
  options.clear();
  option_name.clear();
  PLOGI << "Sub-Weapon Selection scene has been unloaded.";
}

void SubWeaponScene::checkInput() {
  bool key_right = IsKeyPressed(KEY_RIGHT);
  bool key_left = IsKeyPressed(KEY_LEFT);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_right = false;
  bool btn_left = false;

  if (gamepad_detected) {
    btn_right = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    btn_right = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
  }

  if (key_right || btn_right) {
    Menu::nextOption(options, selected_option, true);
  }
  else if (key_left || btn_left) {
    Menu::previousOption(options, selected_option, true);
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
  float offset = 24 * *selected_option;

  float time_elapsed = CURRENT_TIME - start_timestamp;
  float sinY = sinf(time_elapsed * 2.5) * 0.75;

  Vector2 position = {195 + offset, 95 + sinY};
  DrawTextureV(*sprites::weapon_select[2], position, WHITE);
}

void SubWeaponScene::drawWeaponName() {
  int size = fonts::skirmish->baseSize;
  int index = *selected_option;

  string *name = &option_name[index];
  Vector2 position = Text::alignCenter(fonts::skirmish, *name, 
                                       {213, 124}, 1, -3);

  DrawTextEx(*fonts::skirmish, name->c_str(), position, size, -3, 
             COLORS::PALETTE[42]);
}

void SubWeaponScene::drawTechniqueData() {
  int index = *selected_option;

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

void SubWeaponScene::drawScene() {
  DrawTexture(*sprites::weapon_select[0], 0, 80, WHITE);
  DrawTexture(*sprites::weapon_select[1], 96, 144, WHITE);

  drawWeaponIcons();
  drawSelectionArrow();

  drawWeaponName();
  drawTechniqueData();
}

