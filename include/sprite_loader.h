// sprite_loader.h
#pragma once
#include <raylib.h>
#include <toml.hpp>
#include <vector>
#include <string>
#include <cstdint>


/* The sprite loader is responsible for loading all the game's sprites
 * for use. This process is usually done by taking a spritesheet, and
 * parsing every individual sprite within it into it's own texture.
 * The data required to do this, like the path to a specific spritesheet,
 * and area of each sprite is stored within a toml file that's loaded
 * on initizalization.*/
class SpriteLoader {
public:
  SpriteLoader();
  ~SpriteLoader();

  void loadSpritesheet(std::vector<std::string> name_list);
  void parseSprites(std::string sheet_name, Image *spritesheet);

  Rectangle getSpriteArea(toml::value &sprite_data);
  void allocateSprites(std::string sheet_name, int sprite);

private:  
  toml::value meta_data;
  std::vector<Texture> sprites; 
  uint16_t latest_index = 0;
};
