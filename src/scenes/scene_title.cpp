// scenes/scene_title.cpp
#include <raylib.h>
#include <functional>
#include "defaults.h"
#include "globals.h"
#include "scene_title.h"

using std::function;


TitleScene::TitleScene(function<void(int)> load_scene) : Scene(load_scene) 
{

}


void TitleScene::drawScene() {
  int size = fonts::skirmish->baseSize * 2;
  int width = MeasureTextEx(*fonts::skirmish, "True Human Tribulation II", 
                            size, -3).x;
  int half_width = width / 2;
  Vector2 position = {(CANVAS_WIDTH / 2.0f) - half_width, 32};
  DrawTextEx(*fonts::skirmish, "True Human Tribulation II", 
             position, size, -3, COLORS::PALETTE[42]);

}
