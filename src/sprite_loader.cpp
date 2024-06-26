// sprite_loader.cpp
#include <raylib.h>
#include <cstdint>
#include <toml/parser.hpp>
#include <toml/value.hpp>
#include <string>
#include <vector>
#include "globals.h"
#include "sprite_loader.h"
#include <plog/Log.h>

using std::string, std::vector;


SpriteMetaData::SpriteMetaData(string name, uint16_t id) {
  this->name = name;
  this->id = id;
}


SpriteLoader::SpriteLoader() {
  PLOGV << "Loading spritesheet meta data.";
  meta_data = toml::parse("graphics/spritesheets/sheet_data.toml");
  sprites.reserve(32);
}

SpriteLoader::~SpriteLoader() {
  PLOGV << "Unloading all sprites...";
  for (Texture sprite : sprites) {
    UnloadTexture(sprite);
  }

  sprites::player.clear();
  sprites.clear();
  PLOGV << "Sprites have been unloaded.";
}

void SpriteLoader::loadSpritesheet(vector<string> name_list) {
  for (string sheet_name : name_list) {
    PLOGV << "Attempting to parse spritesheet: " << sheet_name;
    string file_path = meta_data[sheet_name]["path"].as_string();

    PLOGI << sheet_name << " spritesheet path: " << file_path;
    Image spritesheet = LoadImage(file_path.c_str());
    parseSprites(sheet_name, spritesheet);

    UnloadImage(spritesheet);
    PLOGV << "Spritesheet successfully parsed!";
  }
}

Rectangle SpriteLoader::getSpriteArea(toml::value &sprite_data) {
  toml::value area_data = sprite_data["area"];

  float x = area_data["x"].as_integer();
  float y = area_data["y"].as_integer();
  float width = area_data["width"].as_integer();
  float height = area_data["height"].as_integer();

  Rectangle sprite_area = {x, y, width, height};
  return sprite_area;
}

void SpriteLoader::parseSprites(string sheet_name, Image &spritesheet) {
  int sprite_count = meta_data[sheet_name]["sprites"].size();
  PLOGI << sheet_name << " sprites detected: " << sprite_count;

  uint16_t old_index = latest_index;

  PLOGI << "Now proceeding to parse and allocate sprites.";
  for (int sprite = 0; sprite < sprite_count; sprite++) {
    toml::value sprite_data = meta_data[sheet_name]["sprites"][sprite];
    
    Rectangle area = getSpriteArea(sprite_data);
    
    Image final_image = ImageCopy(spritesheet);
    ImageCrop(&final_image, area); 
    ImageColorReplace(&final_image, COLORS::PALETTE[0], {0, 0, 0, 0});

    sprites.push_back(LoadTextureFromImage(final_image));
    UnloadImage(final_image);
    latest_index++;
  }

  for (int sprite = old_index; sprite < latest_index; sprite++) {
    allocateSprites(sheet_name, sprite);
  }
}

void SpriteLoader::allocateSprites(string sheet_name, int sprite) {
  if (sheet_name == "player") {
    sprites::player.push_back(&sprites[sprite]);
  }
  else if (sheet_name == "ghoul") {
    sprites::ghoul.push_back(&sprites[sprite]);
  }
  else {
    PLOGW << "There isn't a global pointer list for " << sheet_name <<
    " sprite! Index: " << sprite;
    return;
  }
}
