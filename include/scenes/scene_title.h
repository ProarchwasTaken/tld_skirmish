// scene_title.h
#pragma once
#include <raylib.h>
#include <string>
#include <triblib/base/scene.h>
#include "game.h"


/* The standard issue title screen. As you may have guessed, It displays 
 * the title of the game. Who would've guessed? This is meant to be the
 * second scene to be loaded in regular circumstances.*/
class TitleScene : public Scene {
public:
  TitleScene(Game &skirmish, bool play_music = false);
  ~TitleScene();

  void setupCopyright();
  void setupEnter();

  void updateScene() override;
  void interpolateAlpha();

  void checkInput() override;
  void drawScene() override;
private:
  std::string txt_copyright;
  Vector2 cpr_position;

  std::string txt_enter;
  bool draw_enter = true;
  int enter_size;
  Vector2 enter_position;

  float blink_interval = 0.5;
  float blink_timestamp = 0;

  Color main_tint = WHITE;
  bool fading_out = false;

  float fade_time = 0.5;
  float fade_percentage = 1.0;
};
