// globals.cpp
#include <raylib.h>
#include <vector>
#include <triblib/base/generics.h>
#include <triblib/data/data_sprites.h>
#include <triblib/data/data_sound.h>
#include <triblib/data/data_music.h>
#include "globals.h"

using std::vector;


bool DEBUG_MODE = false;
bool EXIT_GAME = false;
Image GAME_ICON;

namespace settings {
  int framerate = 60;
  bool fullscreen = false;
}

namespace COLORS {
  Color *PALETTE;
}

namespace fonts {
  Font *skirmish;
}

namespace sprites {
  vector<Texture*> player;
  vector<Texture*> ghoul;
  vector<Texture*> hud_life;
  vector<Texture*> hud_morale;
  vector<Texture*> weapon_knife;
  vector<Texture*> wretch;
  vector<Texture*> weapon_ball;
  vector<Texture*> weapon_select;
  vector<Texture*> damned;
  vector<Texture*> weapon_gun;
  vector<Texture*> hud_mainmenu;
  vector<Texture*> hud_menubox;

  vector<SpriteMetaData> plr_metadata;
}

