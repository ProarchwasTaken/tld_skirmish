// utils/stages.cpp
#include <raylib.h>
#include <cassert>
#include <string>
#include <tuple>
#include <vector>
#include <random>
#include <fstream>
#include <triblib/globals.h>
#include "utils_stages.h"
#include <plog/Log.h>

using std::string, std::tuple, std::make_tuple, std::vector,
std::uniform_int_distribution, std::ifstream, std::getline;


tuple<Texture, Texture> Stages::loadStage(std::string name) {
  PLOGI << "Loading stage: '" << name << "'"; 
  const string directory = "graphics/stages/" + name;

  if (DirectoryExists(directory.c_str()) == false) {
    PLOGE << "Stage directory does not exist!";
    throw;
  }

  const string background_path = directory + "/background.png";
  PLOGD << "Background Path: '" << background_path << "'";

  const string overlay_path = directory + "/overlay.png";
  PLOGD << "Overlay Path: '" << overlay_path << "'";

  Texture background = LoadTexture(background_path.c_str());
  Texture overlay = LoadTexture(overlay_path.c_str());

  PLOGI << "Stage has been loaded.";
  return make_tuple(background, overlay);
}

tuple<Texture, Texture> Stages::loadRandomStage() {
  PLOGI << "Loading a random stage...";
  const vector<string> stage_pool = fetchStagePool();
  const int pool_size = stage_pool.size();

  PLOGI << "Valid stages detected: " << pool_size;
  if (pool_size == 0) {
    PLOGE << "No valid stages were found!";
    throw;
  }

  uniform_int_distribution<int> range(0, pool_size - 1);
  const int chosen_stage = range(RNG::generator);

  string stage_name = stage_pool[chosen_stage];
  PLOGI << "Chosen Stage: " << stage_name;

  return loadStage(stage_name);
}

vector<string> Stages::fetchStagePool() {
  PLOGI << "Fetching stage pool...";
  vector<string> valid_stages;
  const string path = "data/stage_pool.txt";

  if (FileExists(path.c_str()) == false) {
    PLOGE << "File: '" << path << "' not found!";
    throw;
  }

  ifstream file(path);
  string current_line;

  while (getline(file, current_line)) {
    PLOGD << "Validating: '" << current_line << "'"; 
    string stage_directory = "graphics/stages/" + current_line;
    
    if (DirectoryExists(stage_directory.c_str())) {
      string stage_name = current_line;
      valid_stages.push_back(stage_name);
    }
    else {
      PLOGE << "Directory for stage: '" << current_line << 
        "' doesn't exist!";
    }
  }

  file.close();
  return valid_stages;
}
