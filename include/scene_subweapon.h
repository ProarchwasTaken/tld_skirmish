// scene_subweapon.h
#pragma once
#include <raylib.h>
#include <array>
#include "base/generics.h"
#include "base/scene.h"
#include "game.h"

#define OPT_KNIFE 0
#define OPT_BALL 1


class SubWeaponScene : public Scene {
public:
  SubWeaponScene(Game &skirmish);
  ~SubWeaponScene();

  void drawWeaponIcons();
  void drawSelectionArrow();
  void drawWeaponName();

  void checkInput() override;
  void drawScene() override;

private:
  std::array<int, 2> icon_ids;

  float start_timestamp = 0;

  menu_options options;
  menu_options_txt option_name;
  menu_options::iterator selected_option;
};
