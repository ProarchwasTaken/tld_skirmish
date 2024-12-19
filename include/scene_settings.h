// scene_settings.h
#include "base/scene.h"
#include "hud_menu.h"
#include "game.h"


class SettingsScene : public Scene {
public:
  SettingsScene(Game &skirmish);

  void updateScene() override;

  void checkInput() override;
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);
};
