// scene_index.h
#pragma once
#include <raylib.h>
#include "base/generics.h"
#include "base/scene.h"
#include "data/data_index.h"
#include "hud_menu.h"
#include "game.h"

#define SKIR_ENTRY_PLR 0
#define SKIR_ENTRY_GOL 1
#define SKIR_ENTRY_WRH 2
#define SKIR_ENTRY_DAM 3


class IndexScene : public Scene {
public:
  IndexScene(Game &skirmish);
  ~IndexScene();

  void updateScene() override;
  void transitionLerp();

  void checkInput() override;

  void drawCursor(Vector2 position);
  void drawOptions();
  void drawEntryDetails();

  /* Due to how raylib works, I can't begin TextureMode for two render
   * textures at once. How I solved it is by momentarily ending 
   * TextureMode for the game canvas, and starting it back up when it's
   * time to draw the description.*/
  void drawEntryDescription(IndexData *entry, int font_size);
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);
  RenderTexture desc_canvas;

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
