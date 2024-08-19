// enemy_wretch.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>
#include "base/combatant.h"
#include "char_player.h"

#define WRH_HP 20
#define WRH_STABILITY 0.45

#define WRH_MIN_PATIENCE 20
#define WRH_MAX_PATIENCE 40

#define WRH_HIT_SCALE (Vector2){18, 56}
#define WRH_HIT_OFFSET (Vector2){-9, -58}

#define WRH_TEX_SCALE (Vector2){128, 64}
#define WRH_TEX_OFFSET (Vector2){-64, -64}


class WretchEnemy : public Combatant {
public:
  WretchEnemy(PlayerCharacter &player, Vector2 position);

  void update() override;
  void draw(Vector2 &camera_target) override;

  PlayerCharacter *player;
private:
  std::vector<int> anim_idle;
  float idle_frametime;

  std::vector<int> anim_retreat;
  float retreat_frametime;

  std::vector<int> anim_death;
  float death_frametime;
};
