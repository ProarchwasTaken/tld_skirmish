// utils/settings.cpp
#include <raylib.h>
#include <toml/get.hpp>
#include <toml/parser.hpp>
#include <toml/value.hpp>
#include "globals.h"
#include "utils_settings.h"
#include <plog/Log.h>

float soundFallback() {
  PLOGE << "Value of 'sfx_volume' not found or invalid!";
  return settings::sfx_volume;
}

float musicFallback() {
  PLOGE << "Value of 'bgm_volume' not found or invalid!";
  return settings::bgm_volume;
}

int framerateFallback() {
  PLOGE << "Value of 'framerate' not found or invalid!";
  return settings::framerate;
}

bool fullscreenFallBack() {
  PLOGE << "Value of 'fullscreen' not found or invalid!";
  return settings::fullscreen;
}


void Settings::load() {
  PLOGI << "Loading settings...";

  if (FileExists("data/settings.toml") == false) {
    PLOGW << "File not found! Sticking with default settings";
    return;
  }

  toml::value file = toml::parse("data/settings.toml");
  PLOGD << "Parsed data/settings.toml";

  float sfx_volume = toml::find_or(file, "sfx_volume", soundFallback());
  float bgm_volume = toml::find_or(file, "bgm_volume", musicFallback());
  int framerate = toml::find_or(file, "framerate", framerateFallback());
  bool fullscreen = toml::find_or(file, "fullscreen", 
                                  fullscreenFallBack());

  PLOGI << "All values have been loaded. Now applying settings.";
  apply(sfx_volume, bgm_volume, framerate, fullscreen);
}

void Settings::apply(float sfx_volume, float bgm_volume, int framerate, 
                     bool fullscreen)
{
  settings::sfx_volume = sfx_volume;
  settings::bgm_volume = bgm_volume;

  settings::framerate = framerate;
  SetTargetFPS(settings::framerate);

  settings::fullscreen = fullscreen;
}
