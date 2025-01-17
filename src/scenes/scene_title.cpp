// scenes/scene_title.cpp
#include <raylib.h>
#include <raymath.h>
#include <string>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include <triblib/utils/text.h>
#include <triblib/utils/music.h>
#include <triblib/utils/sound.h>
#include "scenes/scene_menu.h"
#include "scenes/scene_title.h"
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

  if (inputs != 0 && fading_out == false && IsWindowFocused()) {
    SoundUtils::play("opt_confirm");
    fading_out = true;
  }
}

void TitleScene::updateScene() {
  float time_elapsed = GetTime() - blink_timestamp;

  if (time_elapsed >= blink_interval) {
    draw_enter = !draw_enter;
    blink_timestamp = GetTime();
  }

  if (fading_out) {
    interpolateAlpha();
  }
  else {
    return;
  }

  if (fade_percentage == 0.0) {
    skirmish->loadScene<MenuScene>();
  }
}

void TitleScene::interpolateAlpha() {
  if (fade_percentage != 0) {
    fade_percentage -= GetFrameTime() / fade_time;
    fade_percentage = Clamp(fade_percentage, 0, 1.0);

    main_tint.a = Lerp(0, 255, fade_percentage);
  }
}

void TitleScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  DrawTexture(*sprites::hud_mainmenu[0], 8, 8, main_tint);

  int size = fonts::skirmish->baseSize;  
  DrawTextEx(*fonts::skirmish, txt_copyright.c_str(), cpr_position, size,
             -3, main_tint);

  if (draw_enter && fading_out == false) {
    DrawTextEx(*fonts::skirmish, txt_enter.c_str(), enter_position, 
               size, 0, WHITE);
  }
}
