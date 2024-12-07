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
#define OPT_GUN 2

#define OPT_YES 0
#define OPT_NO 1


/* The Scene where the player will select a sub-weapon to equip before
 * starting the game. This scene also makes use of menu utility functions,
 * in oddly creatives ways to make it work.*/
class SubWeaponScene : public Scene {
public:
  SubWeaponScene(Game &skirmish, bool from_gameover);
  ~SubWeaponScene();

  void updateScene() override;
  void frameInterpolation();
  void windowInterpolation();

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
  bool ready = false;
  bool from_gameover;
  bool going_back = false;

  float ready_time = 0.25;
  float ready_percentage = 0.0;
  float start_timestamp = 0;

  Rectangle frame_dest;
  Vector2 window_position = {96, 240};
  
  std::array<int, 3> icon_ids = {4, 5, 6};

  menu_options weapon_options = {OPT_KNIFE, OPT_BALL, OPT_GUN};
  menu_options_txt weapon_name = {"Birthright", "Cornerman", "Franklin"};
  menu_options::iterator selected_weapon = weapon_options.begin();

  std::array<SubWeaponData, 3> weapon_data {
    SubWeaponData(
      {"<Cleave>", "Perform a quick\nslash attack.", 3},
      {"<Heave>", "Throw the knife.\nCooldown on use.", 5}
    ),
    SubWeaponData(
      {"<Backcourt>", "Perform a juking\nmaneuver.", 5},
      {"<Foul Play>", "Kick the ball.\nCan be repeated.", 5}
    ),
    SubWeaponData(
      {"<Gee Clip>", "Deals continuous\ndamage if held.", 2},
      {"<Conduit>", "Shoots a bolt of\nelectricity.", 4}
    )
  };

  menu_options confirm_options = {OPT_YES, OPT_NO};
  menu_options_txt confirm_names = {"YES", "NO"};
  menu_options::iterator selected_confirm = confirm_options.begin();

  bool confirm = false;
};
