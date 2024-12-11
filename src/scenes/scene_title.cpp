// scenes/scene_title.cpp
#include <raylib.h>
#include <string>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "utils_text.h"
#include "utils_music.h"
#include "scene_menu.h"
#include "scene_title.h"
#include <plog/Log.h>

using std::string;


TitleScene::TitleScene(Game &skirmish, bool play_music) : Scene(skirmish) 
{
  setupCopyright();
  setupEnter();

  skirmish.transition.fadein(1.0, BLACK);
  if (play_music) {
    MusicUtils::play(0);
  }
  PLOGI << "Successfully loaded Title scene.";
}

TitleScene::~TitleScene() {
  PLOGI << "Successfully unloaded Title scene.";
}

void TitleScene::setupCopyright() {
  txt_copyright = "@2024 Tyler Dillard";

  cpr_position = Text::alignRight(fonts::skirmish, txt_copyright.c_str(), 
                                  {CANVAS_WIDTH, 228}, 1, -3);
}

void TitleScene::setupEnter() {
  txt_enter = "PRESS ANY BUTTON";
  enter_position = Text::alignRight(fonts::skirmish, txt_enter.c_str(), 
                                     {426, 218}, 1, 0);
}

void TitleScene::checkInput() {
  int inputs = GetKeyPressed() + GetGamepadButtonPressed();

  if (inputs != 0) {
    skirmish->loadScene<MenuScene>();
    return;
  }
}

void TitleScene::updateScene() {
  float time_elapsed = GetTime() - blink_timestamp;

  if (time_elapsed >= blink_interval) {
    draw_enter = !draw_enter;
    blink_timestamp = GetTime();
  }
}

void TitleScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  DrawTexture(*sprites::hud_mainmenu[0], 8, 8, WHITE);

  int size = fonts::skirmish->baseSize;  
  DrawTextEx(*fonts::skirmish, txt_copyright.c_str(), cpr_position, size,
             -3, WHITE);

  if (draw_enter) {
    DrawTextEx(*fonts::skirmish, txt_enter.c_str(), enter_position, 
               size, 0, WHITE);
  }
}
