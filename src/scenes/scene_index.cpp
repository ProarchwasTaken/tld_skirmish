// scenes/scene_index.cpp
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "base/scene.h"
#include "data/data_index.h"
#include "globals.h"
#include "game.h"
#include "utils_sound.h"
#include "utils_menu.h"
#include "scene_menu.h"
#include "scene_index.h"
#include <plog/Log.h>

using std::vector;
constexpr float BOX_START_Y = 69;
constexpr float BOX_END_Y = 61;
constexpr float DESC_HEIGHT = 76;

vector<IndexData> SKIR_ENTRIES = {
  {"SKIRMISHER", "HUMAN - MID TIER", "ALIVE - ACTIVE", 
    "Experience: C\n"
    "Mental Fortitude: B\n"
    "Intelligence: C\n"
    "Cooperation: D\n"
    "Emotional Intelligence: C\n"
    "Planning: D\n"
    "Quick Thinking: A\n"
    "Versatility: A \n"
  },
  {"GHOUL", "EX-HUMAN", "IRRELEVANT", ""},
  {"WRETCH", "EX-HUMAN", "IRRELEVANT", ""},
  {"DAMNED", "EX-HUMAN", "IRRELEVANT", ""}
};


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

void IndexScene::checkInput() {  
  if (ready == false) {
    return;
  }

  bool key_down = IsKeyPressed(KEY_DOWN);
  bool key_up = IsKeyPressed(KEY_UP);
  bool key_x = IsKeyPressed(KEY_X);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_down = false; 
  bool btn_up = false;
  bool btn_b = false;

  if (gamepad_detected) {
    btn_down = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    btn_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    btn_b = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
  }

  if (key_down || btn_down) {
    Menu::nextOption(skir_options, selected_option, true);
  }
  else if (key_up || btn_up) {
    Menu::previousOption(skir_options, selected_option, true);
  }
  else if (key_x || btn_b) {
    SoundUtils::play("opt_cancel");
    ready = false;
    exiting = true;
  }
}

void IndexScene::drawCursor(Vector2 position) {
  float percentage = (sinf(CURRENT_TIME * 10) * 0.5) + 0.5;

  Color color = WHITE;
  color.a = 255 * percentage;

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
  IndexData *entry = &SKIR_ENTRIES[*selected_option];

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

  EndTextureMode();
  BeginTextureMode(desc_canvas); 
  {
    ClearBackground(BLACK);
    DrawTextEx(*fonts::skirmish, entry->description.c_str(), position, 
               font_size, -3, WHITE);
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
