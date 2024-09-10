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


/* The Scene where the player will select a sub-weapon to equip before
 * starting the game. This scene also makes use of menu utility functions,
 * in oddly creatives ways to make it work.*/
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
  std::array<int, 2> icon_ids = {4, 5};

  float start_timestamp = 0;

  menu_options weapon_options = {OPT_KNIFE, OPT_BALL};
  menu_options_txt weapon_name = {"Birthright", "Cornerman"};
  menu_options::iterator selected_weapon = weapon_options.begin();

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

  menu_options confirm_options = {OPT_YES, OPT_NO};
  menu_options_txt confirm_names = {"YES", "NO"};
  menu_options::iterator selected_confirm = confirm_options.begin();

  bool confirm = false;
};
