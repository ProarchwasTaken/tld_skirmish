// sprite_loader.cpp
#include <raylib.h>
#include <toml/parser.hpp>
#include <toml/value.hpp>
#include <string>
#include <vector>
#include "globals.h"
#include "sprite_loader.h"
#include <plog/Log.h>

using std::string, std::vector;


SpriteMetaData::SpriteMetaData(string name, Texture *sprite) {
  this->name = name;
  this->sprite = sprite;
}


SpriteLoader::SpriteLoader() {
  PLOGV << "Loading spritesheet meta data.";
  meta_data = toml::parse("graphics/spritesheets/sheet_data.toml");
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
  setInitialCapacity(name_list);

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

void SpriteLoader::setInitialCapacity(vector<string> &name_list) {
  int sprite_total = 0;

  for (string sheet_name : name_list) {  
    int sprite_count = meta_data[sheet_name]["sprites"].size();
    sprite_total += sprite_count;
  }

  PLOGI << "Sprite Total: " << sprite_total;
  sprites.reserve(sprite_total);
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

  PLOGI << "Now proceeding to parse and allocate sprites.";
  for (int sprite = 0; sprite < sprite_count; sprite++) {
    toml::value sprite_data = meta_data[sheet_name]["sprites"][sprite];

    string sprite_name = sprite_data["name"].as_string(); 
    Rectangle area = getSpriteArea(sprite_data);
    
    Image final_image = ImageCopy(spritesheet);
    ImageCrop(&final_image, area); 
    ImageColorReplace(&final_image, COLORS::PALETTE[0], {0, 0, 0, 0});

    sprites.push_back(LoadTextureFromImage(final_image));
    UnloadImage(final_image);

    allocateSprite(sheet_name, sprite_name);
    latest_index++;
  }
}

void SpriteLoader::allocateSprite(string sheet_name, string sprite_name) {
  vector<Texture*> *sprite_list;
  vector<SpriteMetaData> *data_list;

  if (sheet_name == "hud_life") {
    sprite_list = &sprites::hud_life;
    data_list = &sprites::plr_metadata;
  }
  else if (sheet_name == "player") {
    sprite_list = &sprites::player;
    data_list = &sprites::plr_metadata;
  }
  else if (sheet_name == "ghoul") {
    sprite_list = &sprites::ghoul;
    data_list = &sprites::gol_metadata;
  }
  else {
    PLOGE << "Sprite pointer list not found!";
    return;
  }

  sprite_list->push_back(&sprites[latest_index]);
  data_list->push_back(
    SpriteMetaData(sprite_name, &sprites[latest_index])
  );
  
  PLOGI << "Successfully Allocated '" << sheet_name << "' sprite: '" << 
    sprite_name << "'.";
}
