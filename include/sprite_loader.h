// sprite_loader.h
#pragma once
#include <raylib.h>
#include <toml.hpp>
#include <memory>
#include <vector>
#include <string>


class SpriteLoader {
public:
  SpriteLoader();
  ~SpriteLoader();

  void loadSpritesheet(std::vector<std::string> name_list);

private:  
  toml::value meta_data;
  std::vector<std::unique_ptr<Texture>> sprites; 
};
