// scene_splash.h
#include <raylib.h>
#include <string>
#include "base/scene.h"
#include "game.h"


class SplashScene : public Scene {
public:
  SplashScene(Game &skirmish);
  ~SplashScene();

  void updateScene() override;
  void interpolationAlpha();

  void checkInput() override;
  
  void drawScene() override;
private:
  Texture icon;
  Color tint = {255, 255, 255, 0};

  std::string text = "\"Our Birthright as players in this game of life"
  " could never be fully extinquished\".";
  Vector2 txt_position = {213, 120};

  float alpha_percentage = 0.0;
  float fade_time = 1.0;

  float start_timestamp = 0;
  float start_time = 8.0;
};
