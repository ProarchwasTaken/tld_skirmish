// scene_title.h
#pragma once
#include <raylib.h>
#include <string>
#include <functional>
#include "base/scene.h"


class TitleScene : public Scene {
public:
  TitleScene(std::function<void(int)> load_scene);
  ~TitleScene();

  void setupTitle();
  void setupCopyright();
  void setupEnter();

  void checkInput() override;
  void drawScene() override;
private:
  std::string txt_title;
  Vector2 title_position;

  std::string txt_copyright;
  Vector2 cpr_position;

  std::string txt_enter;
  int enter_size;
  Vector2 enter_position;
};
