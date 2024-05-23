// sprite_loader.cpp
#include <raylib.h>
#include <toml.hpp>
#include <toml/parser.hpp>
#include <memory>
#include <string>
#include <vector>
#include "sprite_loader.h"
#include <plog/Log.h>

using std::string, std::vector;


SpriteLoader::SpriteLoader() {
  PLOGI << "Loading spritesheet meta data.";
  meta_data = toml::parse("graphics/spritesheets/sheet_data.toml");
}

void SpriteLoader::loadSpritesheet(vector<string> name_list) {
  for (string name : name_list) {
    PLOGI << "Attempting to parse spritesheet: " << name;
  }
}
