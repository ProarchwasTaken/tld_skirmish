// utils_settings.h
#pragma once


namespace Settings {
  void save();
  void load();
  void apply(float sfx_volume, float bgm_volume, int framerate, 
             bool fullscreen);
}
