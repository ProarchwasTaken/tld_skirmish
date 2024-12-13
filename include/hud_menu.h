// hud_menu.h
#include <raylib.h>


class MenuHud {
public:
  MenuHud(bool interpolate);

  void update();

  void drawTopBar();
  void draw();
private:
  bool interpolate;
};
