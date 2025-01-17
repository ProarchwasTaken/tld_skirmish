// utils_stages.h
#pragma once
#include <raylib.h>
#include <string>
#include <vector>
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

  /* Helper function for fetching and returning the stage pool of which
   * is an external file containing the names for every stage in the game
   * except the debug stage.*/
  std::vector<std::string> fetchStagePool(); 
  
  /* Returns the textures associated with a random stage from the stage
   * pool. Makes use of the prior two functions defined before, and it's
   * basically a wrapper for loadStage.*/
  std::tuple<Texture, Texture> loadRandomStage();
}

