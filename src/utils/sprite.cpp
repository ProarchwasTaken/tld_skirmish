// utils/sprite.cpp
#include <cstddef>
#include <raylib.h>
#include <string>
#include <vector>
#include "sprite_loader.h"
#include "utils.h"
#include <plog/Log.h>

using std::string, std::vector;


Texture *Sprite::getSprite(string sprite_name, 
                           vector<SpriteMetaData> &data_list) 
{
  Texture *sprite = NULL;

  for (auto data : data_list) {
    if (data.name == sprite_name) {
      sprite = data.sprite;
    }
  }

  if (sprite == NULL) {
    PLOGE << "Sprite not found!";
    throw;
  }

  return sprite;
}
