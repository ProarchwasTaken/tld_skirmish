// scene_splash.h
#include <raylib.h>
#include <string>
#include <triblib/base/scene.h>
#include "game.h"
#include "sys_transition.h"


class SplashScene : public Scene {
public:
  SplashScene(Game &skirmish);
  ~SplashScene();

  void updateScene() override;
  void splashSequence(float time_elapsed);
  void interpolateAlpha();
  void interpolateSize();

  void checkInput() override;
  
  void drawScene() override;
private:
  Texture icon;
  Vector2 icon_position = {117, 24};
  Color icon_tint = {255, 255, 255, 0};

  std::string text = "\"Our Birthright as players in this game of life"
  " could never be fully extinquished.\"";
  bool draw_text = true;
  Vector2 txt_position = {213, 114};

  float alpha_percentage = 0.0;
  float icon_fade_time = 1.0;

  float line_size = 0; 
  float max_line_size = 16;

  float size_percentage = 0.0;
  float size_time = 1.0;

  WipeTransition bg_transition;
  bool bg_fading_out = false;

  float start_timestamp = 0;
  float start_time = 10.0;
};
