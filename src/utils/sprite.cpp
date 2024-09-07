// utils/sprite.cpp
#include <raylib.h>
#include <string>
#include <cstddef>
#include <cassert>
#include <vector>
#include "data/data_sprites.h"
#include "utils.h"
#include <plog/Log.h>

using std::string, std::vector;


Texture *Sprite::getSprite(string sprite_name, 
                           vector<SpriteMetaData> &data_list) 
{
  PLOGD << "Attempting to get sprite: " << sprite_name;
  Texture *sprite = NULL;

  for (auto data : data_list) {
    if (data.name == sprite_name) {
      sprite = data.sprite;
      break;
    }
  }
  assert(sprite != NULL && "Sprite not found!");

  return sprite;
}
