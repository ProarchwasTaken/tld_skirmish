// sprite_loader.h
#pragma once
#include <raylib.h>
#include <toml.hpp>
#include <vector>
#include <string>
#include <cstdint>


class SpriteLoader {
public:
  SpriteLoader();
  ~SpriteLoader();

  void loadSpritesheet(std::vector<std::string> name_list);
  Rectangle getSpriteArea(toml::value &sprite_data);
  void parseSprites(std::string sheet_name, Image *spritesheet);
  void allocateSprites(std::string sheet_name, int sprite);

private:  
  toml::value meta_data;
  std::vector<Texture> sprites; 
  uint16_t latest_index = 0;
};
