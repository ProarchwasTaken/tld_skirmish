// utils/settings.cpp
#include <fstream>
#include <string>
#include <raylib.h>
#include <toml/get.hpp>
#include <toml/parser.hpp>
#include <toml/serializer.hpp>
#include <toml/value.hpp>
#include "globals.h"
#include "utils_settings.h"
#include <plog/Log.h>

using std::string, std::ofstream;


void Settings::save() {
  PLOGI << "Saving settings to 'data/settings.toml'";

  const toml::value data = {
    {"sfx_volume", settings::sfx_volume},
    {"bgm_volume", settings::bgm_volume},
    {"framerate", settings::framerate},
    {"fullscreen", settings::fullscreen}
  };

  ofstream file("data/settings.toml");
  
  PLOGD << "Writing to file...";
  file << toml::format(data);
  file.close();

  PLOGI << "Settings saved successfully.";
}

void Settings::load() {
  PLOGI << "Loading settings...";

  if (FileExists("data/settings.toml") == false) {
    PLOGW << "File not found! Sticking with default settings";
    SetTargetFPS(settings::framerate);
    return;
  }

  toml::value file = toml::parse("data/settings.toml");
  PLOGD << "Parsed data/settings.toml";

  float sfx_volume = toml::find_or(file, "sfx_volume", 1.0);
  float bgm_volume = toml::find_or(file, "bgm_volume", 1.0);
  int framerate = toml::find_or(file, "framerate", 60);
  bool fullscreen = toml::find_or(file, "fullscreen", false);

  PLOGI << "All values have been loaded. Now applying settings.";
  apply(sfx_volume, bgm_volume, framerate, fullscreen);
}

void Settings::apply(float sfx_volume, float bgm_volume, int framerate, 
                     bool fullscreen)
{
  PLOGD << "Sound Volume: " << sfx_volume;
  settings::sfx_volume = sfx_volume;

  PLOGD << "Music Volume: " << bgm_volume;
  settings::bgm_volume = bgm_volume;

  PLOGD << "Framerate: " << framerate;
  settings::framerate = framerate;
  SetTargetFPS(settings::framerate);

  PLOGD << "Fullscreen: " << fullscreen;
  settings::fullscreen = fullscreen;
}
