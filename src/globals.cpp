// globals.cpp
#include <raylib.h>
#include <vector>
#include <random>
#include "globals.h"
#include "base/generics.h"
#include "audio.h"
#include "sprite_loader.h"

using std::vector, std::mt19937, std::random_device;


bool DEBUG_MODE = false;
bool EXIT_GAME = false;
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

  vector<SpriteMetaData> plr_metadata;
}

namespace audio {
  vector<SoundMetaData> sfx_metadata;
}

namespace RNG {
  mt19937 generator(random_device{}());
}

namespace Dynamic {
  dynamic_list queue;
}

