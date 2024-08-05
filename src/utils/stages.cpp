// utils/stages.cpp
#include <raylib.h>
#include <cassert>
#include <string>
#include <tuple>
#include "utils.h"
#include <plog/Log.h>

using std::string, std::tuple, std::make_tuple;


tuple<Texture, Texture> Stages::loadStage(std::string name) {
  PLOGI << "Loading stage: '" << name << "'"; 
  string directory = "graphics/stages/" + name;

  assert(DirectoryExists(directory.c_str()) && 
         "Stage directory does not exist!");

  string background_path = directory + "/background.png";
  PLOGD << "Background Path: '" << background_path << "'";

  string overlay_path = directory + "/overlay.png";
  PLOGD << "Overlay Path: '" << overlay_path << "'";

  Texture background = LoadTexture(background_path.c_str());
  Texture overlay = LoadTexture(overlay_path.c_str());

  PLOGI << "Stage has been loaded.";
  return make_tuple(background, overlay);
}
