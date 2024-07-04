// globals.cpp
#include <raylib.h>
#include <vector>
#include <random>
#include "globals.h"
#include "audio.h"
#include "sprite_loader.h"

using std::vector, std::mt19937, std::random_device;


bool DEBUG_MODE = false;

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

  vector<SpriteMetaData> plr_metadata;
  vector<SpriteMetaData> gol_metadata;
  vector<SpriteMetaData> hud_life_metadata;
}

namespace audio {
  vector<SoundMetaData> sfx_metadata;
}

namespace RNG {
  mt19937 generator(random_device{}());
}

