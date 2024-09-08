// utils_stages.h
#pragma once
#include <raylib.h>
#include <string>
#include <tuple>


/* Stages are the space of which all combatants will fight on. They are
 * not meant to physically affect the game, but instead for cosmetic
 * flavor. Stages are not exactly a class, they're represented by a set
 * of textures, and these functions are for loading and returning them.*/
namespace Stages {
  /* For getting the textures associated with a stage and returning them
   * as a tuple that could be unpacked. Each stage is defined by a 
   * directory of which their files are located.*/
  std::tuple<Texture, Texture> loadStage(std::string name);
}

