// scene_subweapon.h
#pragma once
#include <raylib.h>
#include <array>
#include "base/generics.h"
#include "base/scene.h"
#include "data/data_weapon.h"
#include "game.h"

#define OPT_KNIFE 0
#define OPT_BALL 1

#define OPT_YES 0
#define OPT_NO 1


class SubWeaponScene : public Scene {
public:
  SubWeaponScene(Game &skirmish);
  ~SubWeaponScene();

  void drawWeaponIcons();
  void drawSelectionArrow();

  void drawWeaponName();
  void drawTechniqueData();
  void drawTechLine(TechData &tech, Vector2 position);

  void drawWarningText();
  void drawConfirmOptions();

  void checkInput() override;
  void checkConfirmInput(std::array<bool, 4> key_input,
                         std::array<bool, 4> btn_input);
  void selectConfirmOption();

  void drawScene() override;

private:
  std::array<int, 2> icon_ids;

  float start_timestamp = 0;

  menu_options weapon_options;
  menu_options_txt weapon_name;
  menu_options::iterator selected_weapon;

  std::array<SubWeaponData, 2> weapon_data {
    SubWeaponData(
      {"<Cleave>", "Perform a quick\nslash attack.", 3},
      {"<Heave>", "Throw the knife.\nCooldown on use.", 5}
    ),
    SubWeaponData(
      {"<Backcourt>", "Perform a juking\nmaneuver.", 5},
      {"<Foul Play>", "Kick the ball.\nCan be repeated.", 5}
    )
  };

  menu_options confirm_options;
  menu_options_txt confirm_names;
  menu_options::iterator selected_confirm;

  bool confirm = false;
};
