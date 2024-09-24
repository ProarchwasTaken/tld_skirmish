// utils_sprite.h
#pragma once
#include <vector>
#include <string>
#include "data/data_sprites.h"

namespace Sprite {
  /* This is for getting a specific sprite by name rather than using an
   * index. This is typically used by action commands that are meant to
   * to used by multiple different combatants.*/
  Texture *getSprite(std::string sprite_name, 
                     std::vector<SpriteMetaData> &data_list);
}

