// scene_index.h
#pragma once
#include "base/generics.h"
#include "base/scene.h"
#include "hud_menu.h"
#include "game.h"

#define SKIR_ENTRY_PLR 0
#define SKIR_ENTRY_GOL 1
#define SKIR_ENTRY_WRH 2
#define SKIR_ENTRY_DAM 3


class IndexScene : public Scene {
public:
  IndexScene(Game &skirmish);

  void updateScene() override;
  void transitionLerp();

  void checkInput() override;

  void drawCursor(Vector2 position);
  void drawOptions();
  void drawEntryDetails();
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);

  bool ready = false;
  bool exiting = false;

  float ready_time = 0.25;
  float ready_percentage = 0.0;

  float box_y = 69;
  Color box_tint = {255, 255, 255, 0};

  menu_options skir_options = {
    SKIR_ENTRY_PLR,
    SKIR_ENTRY_GOL,
    SKIR_ENTRY_WRH,
    SKIR_ENTRY_DAM
  };

  menu_options::iterator selected_option = skir_options.begin();
};
