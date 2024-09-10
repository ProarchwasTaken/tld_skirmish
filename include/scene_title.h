// scene_title.h
#pragma once
#include <raylib.h>
#include <string>
#include "base/scene.h"
#include "game.h"


/* The standard issue title screen. As you may have guessed, It displays 
 * the title of the game. Who would've guessed? This is meant to be the
 * second scene to be loaded in regular circumstances.*/
class TitleScene : public Scene {
public:
  TitleScene(Game &skirmish);
  ~TitleScene();

  void setupTitle();
  void setupCopyright();
  void setupEnter();

  void checkInput() override;
  void updateScene() override;
  void drawScene() override;
private:
  std::string txt_title;
  Vector2 title_position;

  std::string txt_copyright;
  Vector2 cpr_position;

  std::string txt_enter;
  bool draw_enter = true;
  int enter_size;
  Vector2 enter_position;

  float blink_interval = 0.5;
  float blink_timestamp = 0;
};
