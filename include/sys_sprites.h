// sys_sprites
#pragma once
#include <raylib.h>
#include <toml/value.hpp>
#include <vector>
#include <string>
#include <cstdint>

#define SHEET_PLAYER 0
#define SHEET_GHOUL 1
#define SHEET_LIFE 2
#define SHEET_MORALE 3
#define SHEET_KNIFE 4
#define SHEET_WRETCH 5
#define SHEET_BALL 6
#define SHEET_WPN_SELECT 7
#define SHEET_DAMNED 8
#define SHEET_GUN 9
#define SHEET_MAINMENU 10


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

  /* The root method. Everything else are helpers to get the job of
   * loading the game's sprites. This only meant to be called once.
   * Any more, and it would obviously cause problems.*/
  void loadSpritesheet(std::vector<std::string> name_list);

  /* Whenever a vector increases in capacity, the addresses to every item
   * in the container will chance. Given how things work with how every
   * sprite will have a pointer assigned to them, this could be very 
   * problematic. This method is programmed to avoid this issue by 
   * counting the total number of sprites that will be loaded, and 
   * that as the capacity ahead of time.*/
  void setInitialCapacity(std::vector<std::string> &name_list);

  /* This could be seen as the second level. Parses and allocates every
   * sprite within a spritesheet using the data from the toml file.*/
  void parseSprites(std::string sheet_name, Image &spritesheet);

  /* Returns the dimensions and area of the sprite as a raylib Rectangle.
   * This is one of the necessary functions needed in the process of 
   * sprite parsing.*/
  Rectangle getSpriteArea(toml::value &sprite_data);

  /* Allocates a sprite and it's data to one of the many global sprite
   * lists. This makes it so the sprite is accessible from any part of
   * the program.*/
  void allocateSprite(const int sheet_id, std::string sprite_name);
private:  
  std::vector<Texture> sprites; 
  uint16_t latest_index = 0;
};
