// scenes/scene_title.cpp
#include <raylib.h>
#include <functional>
#include <string>
#include "defaults.h"
#include "globals.h"
#include "scene_title.h"
#include <plog/Log.h>

using std::function, std::string;


TitleScene::TitleScene(function<void(int)> load_scene) : Scene(load_scene) 
{
  PLOGV << "Loading Title scene.";
  setupTitle();
  setupCopyright();
  setupEnter();
  PLOGV << "Title scene has been successfully loaded.";
}

void TitleScene::setupTitle() {
  txt_title = "True Human Tribulation II";

  title_size = fonts::skirmish->baseSize * 2;
  int title_width = MeasureTextEx(*fonts::skirmish, txt_title.c_str(), 
                                  title_size, -3).x;

  title_position.x = (CANVAS_WIDTH / 2.0f) - title_width / 2.0;
  title_position.y = 32;
}

void TitleScene::setupCopyright() {
  txt_copyright = "@2024 Tyler Dillard";

  cpr_size = fonts::skirmish->baseSize;
  int cpr_width = MeasureTextEx(*fonts::skirmish, txt_copyright.c_str(), 
                                cpr_size, -3).x;

  cpr_position.x = CANVAS_WIDTH - cpr_width;
  cpr_position.y = 228;
}

void TitleScene::setupEnter() {
  if (IsGamepadAvailable(0)) {
    txt_enter = "PRESS START";
  }
  else {
    txt_enter = "PRESS ENTER";
  }

  enter_size = fonts::skirmish->baseSize;
  int enter_width = MeasureTextEx(*fonts::skirmish, txt_enter.c_str(), 
                                  enter_size, 0).x;

  enter_position.x = (CANVAS_WIDTH / 2.0f) - enter_width / 2.0;
  enter_position.y = 160;
}

void TitleScene::drawScene() {
  DrawTextEx(*fonts::skirmish, txt_title.c_str(), title_position, 
             title_size, -3, COLORS::PALETTE[42]);
  DrawTextEx(*fonts::skirmish, txt_enter.c_str(), enter_position, 
             enter_size, 0, WHITE);
  DrawTextEx(*fonts::skirmish, txt_copyright.c_str(), cpr_position, 
             cpr_size, -3, WHITE);
}
