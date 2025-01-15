// scenes/scene_index.cpp
#include <cstdint>
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <triblib/base/scene.h>
#include "data/data_index.h"
#include "globals.h"
#include "game.h"
#include <triblib/utils/sound.h>
#include "utils_menu.h"
#include "scene_menu.h"
#include "scene_index.h"
#include <plog/Log.h>

constexpr float BOX_START_Y = 69;
constexpr float BOX_END_Y = 61;
constexpr float DESC_HEIGHT = 76;


IndexScene::IndexScene(Game &skirmish) : Scene(skirmish) {
  desc_canvas = LoadRenderTexture(171, DESC_HEIGHT);
  PLOGI << "Loaded Combatant Index scene.";
}

IndexScene::~IndexScene() {
  UnloadRenderTexture(desc_canvas);
}

void IndexScene::updateScene() {
  menu_hud.update();

  if (ready == false) {
    transitionLerp();
  }
  else if (desc_scrolling) {
    scrolling();
  }
  else if (exiting) {
    skirmish->loadScene<MenuScene>(menu_hud);
  }

}

void IndexScene::transitionLerp() {
  if (exiting == false) {
    ready_percentage += GetFrameTime() / ready_time;
  }
  else {
    ready_percentage -= GetFrameTime() / ready_time; 
  }

  ready_percentage = Clamp(ready_percentage, 0.0, 1.0);

  box_y = Lerp(BOX_END_Y, BOX_START_Y, 1.0 - ready_percentage);
  box_tint.a = Lerp(0, 255, ready_percentage);

  if (ready_percentage == 0.0 || ready_percentage == 1.0) {
    ready = true;
  }
}

void IndexScene::scrolling() {
  int8_t direction;
  if (scrolling_down == scrolling_up) {
    return;
  }
  else if (scrolling_down) {
    direction = 1;
  }
  else {
    direction = -1;
  }

  IndexData *entry = &skir_entries[*selected_option];
  int size = fonts::skirmish->baseSize;
  float desc_height = MeasureTextEx(*fonts::skirmish, 
                                    entry->description.c_str(), 
                                    size, -3).y;

  float max_progress = desc_height - DESC_HEIGHT;
  if (max_progress < 0) {
    max_progress = 0;
  }

  entry->desc_progress += (scroll_speed * DELTA_TIME) * direction;
  entry->desc_progress = Clamp(entry->desc_progress, 0, max_progress);
}

void IndexScene::checkInput() {  
  if (ready == false) {
    return;
  }

  bool key_down = IsKeyPressed(KEY_DOWN);
  bool key_up = IsKeyPressed(KEY_UP);
  bool key_z = IsKeyPressed(KEY_Z);
  bool key_x = IsKeyPressed(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_down = false; 
  bool btn_up = false;
  bool btn_a = false;
  bool btn_b = false;

  if (gamepad_detected) {
    btn_down = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    btn_a = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
  }

  if (desc_scrolling) {
    descInput(key_x || btn_b, key_down || btn_down, key_up || btn_up,
              gamepad_detected);
    return;
  }

  if (key_down || btn_down) {
    Menu::nextOption(skir_options, selected_option, true);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(skir_options, selected_option, true);
  }
  else if (key_z || btn_a) {
    SoundUtils::play("opt_confirm");
    desc_scrolling = true;

  }
  else if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    ready = false;
    exiting = true;
  }
}

void IndexScene::descInput(bool input_back, bool input_down,
                           bool input_up, bool gamepad_detected) 
{
  if (input_back) {
    SoundUtils::play("opt_cancel");
    desc_scrolling = false;
    return;
  }

  if (input_down) {
    scrolling_down = true;
  }
  else if (input_up){
    scrolling_up = true;
  }

  bool key_down = IsKeyReleased(KEY_DOWN);
  bool key_up = IsKeyReleased(KEY_UP);

  bool btn_down = false;
  bool btn_up = false;

  if (gamepad_detected) {
    btn_down = IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
  }

  if (key_down || btn_down) {
    scrolling_down = false;
  }
  else if (key_up || btn_up) {
    scrolling_up = false;
  }
}

void IndexScene::drawCursor(Vector2 position) {
  Color color = WHITE;
  if (desc_scrolling == false) {
    float percentage = (sinf(CURRENT_TIME * 10) * 0.5) + 0.5;
    color.a = 255 * percentage;
  }

  position = Vector2Add(position, {-8, 3});
  DrawTextureV(*sprites::hud_menubox[3], position, color);
}

void IndexScene::drawOptions() {
  int size = fonts::skirmish->baseSize;
  Vector2 position = {120, 68};

  for (int option : skir_options) {
    const char* text = TextFormat("%03i", option + 1);

    Color color;
    if (*selected_option == option) {
      color = COLORS::PALETTE[42];
      drawCursor(position);
    }
    else {
      color = WHITE;
    }

    DrawTextEx(*fonts::skirmish, text, position, size, 0, color);
    position.y += 11;
  }
}

void IndexScene::drawEntryDetails() {
  int size = fonts::skirmish->baseSize;
  IndexData *entry = &skir_entries[*selected_option];

  DrawTextEx(*fonts::skirmish, entry->subject_name.c_str(), {185, 68}, 
             size, -3, WHITE);
  DrawTextEx(*fonts::skirmish, entry->subject_class.c_str(), {192, 79},
             size, -3, WHITE);
  DrawTextEx(*fonts::skirmish, entry->subject_status.c_str(), {201, 90}, 
             size, -3, WHITE);

  drawEntryDescription(entry, size);
}

void IndexScene::drawEntryDescription(IndexData *entry, int font_size) {
  Vector2 position = {0,  -1 - entry->desc_progress};
  Color desc_color;

  if (desc_scrolling) {
    desc_color = WHITE;
  }
  else {
    desc_color = COLORS::PALETTE[40];
  }

  EndTextureMode();
  BeginTextureMode(desc_canvas); 
  {
    ClearBackground(BLACK);
    DrawTextEx(*fonts::skirmish, entry->description.c_str(), position, 
               font_size, -3, desc_color);
  }
  EndTextureMode();

  Rectangle source = {0, 0, 171, -DESC_HEIGHT};
  Rectangle dest = {145, 102, 171, DESC_HEIGHT};

  BeginTextureMode(skirmish->canvas);
  DrawTexturePro(desc_canvas.texture, source, dest, {0.0}, 0, WHITE);
}

void IndexScene::drawScene() {
  DrawTexture(skirmish->bg_main, 0, 0, WHITE);
  menu_hud.draw();
  DrawTexture(*sprites::hud_mainmenu[7], 4, 21, WHITE);

  DrawTexture(*sprites::hud_menubox[2], 109, box_y, box_tint);

  if (ready && exiting == false) {
    drawOptions();
    drawEntryDetails();
  }
}
