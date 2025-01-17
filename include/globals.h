// globals.h
/* This file contains a number of macros and external variables that all
 * have the capacity to be different depending on the circumstances. Like
 * what command line arguments are used when the game is ran, or what
 * platform the game is running on.*/
#pragma once
#include <raylib.h>
#include <vector>
#include <triblib/globals.h>
#include <triblib/base/generics.h>
#include <triblib/data/data_sprites.h>
#include <triblib/data/data_sound.h>
#include <triblib/data/data_music.h>
#include "defaults.h"

extern bool DEBUG_MODE;
extern bool EXIT_GAME;
extern Image GAME_ICON;

#define DELTA_TIME (GetFrameTime() * TARGET_FRAMERATE)

namespace settings {
  extern int framerate;
  extern bool fullscreen;
}

namespace COLORS {
  extern Color *PALETTE;
}

namespace fonts {
  extern Font *skirmish;
}

namespace sprites {
  extern std::vector<Texture*> player;
  extern std::vector<SpriteMetaData> plr_metadata;

  extern std::vector<Texture*> ghoul;
  extern std::vector<Texture*> wretch;
  extern std::vector<Texture*> damned;

  extern std::vector<Texture*> weapon_knife;
  extern std::vector<Texture*> weapon_ball;
  extern std::vector<Texture*> weapon_gun;

  extern std::vector<Texture*> hud_life;
  extern std::vector<Texture*> hud_morale;
  extern std::vector<Texture*> hud_mainmenu;
  extern std::vector<Texture*> hud_menubox;

  extern std::vector<Texture*> weapon_select;
}

