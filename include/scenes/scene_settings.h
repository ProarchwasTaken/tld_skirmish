// scene_settings.h
#include <cstdint>
#include <triblib/base/generics.h>
#include <triblib/base/scene.h>
#include "hud/hud_menu.h"
#include "game.h"

#define OPT_SOUND 0
#define OPT_MUSIC 1
#define OPT_FRAMERATE 2
#define OPT_FULLSCREEN 3
#define OPT_APPLY 4


class SettingsScene : public Scene {
public:
  SettingsScene(Game &skirmish);

  void updateScene() override;
  void transitionLerp();

  void checkInput() override;
  void incrementSetting();
  void decrementSetting();
  void applyOptionSelected();

  void drawOption(uint8_t option, float current_y);
  void drawMenuOptions();
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);

  bool ready = false;
  bool exiting = false;

  float ready_time = 0.25;
  float ready_percentage = 0.0;

  float box_y = 65;
  Color box_tint = {255, 255, 255, 0};

  int sfx_volume;
  int bgm_volume;
  int framerate;
  bool fullscreen;

  menu_options options = {
    OPT_SOUND,
    OPT_MUSIC,
    OPT_FRAMERATE,
    OPT_FULLSCREEN,
    OPT_APPLY
  };

  menu_options::iterator selected_option = options.begin();
};
