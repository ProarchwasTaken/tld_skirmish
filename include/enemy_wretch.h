// enemy_wretch.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>
#include "base/combatant.h"
#include "char_player.h"

#define WRH_HP 20
#define WRH_STABILITY 0.45

#define WRH_MIN_PATIENCE 60
#define WRH_MAX_PATIENCE 80

#define WRH_HIT_SCALE (Vector2){18, 56}
#define WRH_HIT_OFFSET (Vector2){-9, -58}

#define WRH_TEX_SCALE (Vector2){128, 64}
#define WRH_TEX_OFFSET (Vector2){-64, -64}


class WretchEnemy : public Combatant {
public:
  WretchEnemy(PlayerCharacter &player, Vector2 position);

  void update() override;
  void draw(Vector2 &camera_target) override;
  void drawDebug() override;

  void neutralBehavior();
  void pursue();

  PlayerCharacter *player;
private:
  float movement_speed;

  uint16_t player_dist;
  uint16_t preferred_dist;

  uint8_t retreat_patience = 0;
  float tick_timestamp = 0;

  std::vector<int> anim_idle;
  float idle_frametime;

  std::vector<int> anim_retreat;
  float retreat_frametime;

  std::vector<int> anim_death;
  float death_frametime;
};
