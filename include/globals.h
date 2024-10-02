// globals.h
/* This file contains a number of macros and external variables that all
 * have the capacity to be different depending on the circumstances. Like
 * what command line arguments are used when the game is ran, or what
 * platform the game is running on.*/
#pragma once
#include <raylib.h>
#include <vector>
#include <random>
#include "defaults.h"
#include "base/generics.h"
#include "data/data_sprites.h"
#include "data/data_sound.h"

extern bool DEBUG_MODE;
extern bool EXIT_GAME;

extern double PAUSE_PENALTY;
#define CURRENT_TIME (GetTime() - PAUSE_PENALTY)
#define DELTA_TIME (GetFrameTime() * TARGET_FRAMERATE)

namespace COLORS {
  extern Color *PALETTE;
}

namespace fonts {
  extern Font *skirmish;
}

namespace audio {
  extern std::vector<SoundMetaData> sfx_metadata;
}

namespace sprites {
  extern std::vector<Texture*> player;
  extern std::vector<SpriteMetaData> plr_metadata;

  extern std::vector<Texture*> ghoul;
  extern std::vector<Texture*> wretch;
  extern std::vector<Texture*> damned;

  extern std::vector<Texture*> weapon_knife;
  extern std::vector<Texture*> weapon_ball;

  extern std::vector<Texture*> hud_life;
  extern std::vector<Texture*> hud_morale;

  extern std::vector<Texture*> weapon_select;
}

namespace RNG {
  extern std::mt19937 generator;
}


namespace Dynamic {
  extern dynamic_list queue;
}
