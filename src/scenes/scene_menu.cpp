// scenes/scene_menu.cpp
#include <raylib.h>
#include <string>
#include <functional>
#include <cstdint>
#include "globals.h"
#include "utils.h"
#include "scene_menu.h"
#include <plog/Log.h>

using std::function, std::string;


MenuScene::MenuScene(function<void(int)> load_scene) : Scene(load_scene) {
  PLOGV << "Loading MainMenu scene.";
  options = {
    OPT_PLAY,
    OPT_SETTINGS,
    OPT_CONTROLS,
    OPT_QUIT
  };
  options_text = {
    "PLAY GAME",
    "SETTINGS",
    "CONTROLS",
    "QUIT GAME"
  };

  selected_option = options.begin();
}

void MenuScene::drawScene() {
  int size = fonts::skirmish->baseSize;
  float current_y = 90;

  for (uint8_t option : options) {
    string *text = &options_text[option];
    Vector2 position = Text::alignCenter(fonts::skirmish, *text, 
                                         {213, current_y}, 1, 0);

    Color color;
    if (*selected_option == option) {
      color = COLORS::PALETTE[22];
    }
    else {
      color = WHITE;
    }

    DrawTextEx(*fonts::skirmish, text->c_str(), position, size, 0, color);
    current_y += size;
  }
}
