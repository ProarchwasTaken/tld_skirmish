// scenes/scene_title.cpp
#include <raylib.h>
#include <functional>
#include <string>
#include "defaults.h"
#include "globals.h"
#include "utils.h"
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

  title_position = Text::alignCenter(fonts::skirmish, txt_title, 
                                     {213, 32}, 2, -3);
}

void TitleScene::setupCopyright() {
  txt_copyright = "@2024 Tyler Dillard";

  cpr_position = Text::alignRight(fonts::skirmish, txt_copyright.c_str(), 
                                  {CANVAS_WIDTH, 228}, 1, -3);
}

void TitleScene::setupEnter() {
  if (IsGamepadAvailable(0)) {
    txt_enter = "PRESS START";
  }
  else {
    txt_enter = "PRESS ENTER";
  }

  enter_position = Text::alignCenter(fonts::skirmish, txt_enter.c_str(), 
                                     {213, 160}, 1, 0);
}

void TitleScene::drawScene() {
  int size = fonts::skirmish->baseSize;
  
  DrawTextEx(*fonts::skirmish, txt_title.c_str(), title_position, 
             size * 2, -3, COLORS::PALETTE[42]);
  DrawTextEx(*fonts::skirmish, txt_enter.c_str(), enter_position, size, 0,
             WHITE);
  DrawTextEx(*fonts::skirmish, txt_copyright.c_str(), cpr_position, size,
             -3, WHITE);
}