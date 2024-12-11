// globals.cpp
#include <raylib.h>
#include <vector>
#include <random>
#include "globals.h"
#include "base/generics.h"
#include "data/data_sprites.h"
#include "data/data_sound.h"
#include "data/data_music.h"

using std::vector, std::mt19937, std::random_device;


bool DEBUG_MODE = false;
bool EXIT_GAME = false;
Image GAME_ICON;
double PAUSE_PENALTY = 0;

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

  vector<SpriteMetaData> plr_metadata;
}

namespace audio {
  vector<SoundMetaData> sfx_metadata;
  vector<MusicMetaData> bgm_metadata;

  Music music_stream;
  MusicMetaData *stream_data = NULL;
  VolumeLerpData bgm_vol_lerp;
}

namespace RNG {
  mt19937 generator(random_device{}());
}

namespace Dynamic {
  dynamic_list queue;
}

