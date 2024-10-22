// enemy_damned.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/combatant.h"
#include "char_player.h"

#define DAM_HP 25
#define DAM_STABILITY 0.50

#define DAM_HIT_SCALE (Vector2){18, 56}
#define DAM_HIT_OFFSET (Vector2){-9, -58}

#define DAM_TEX_SCALE (Vector2){96, 64}
#define DAM_TEX_OFFSET (Vector2){-48, -64}


class DamnedEnemy : public Combatant {
public:
  DamnedEnemy(PlayerCharacter &player, Vector2 position);
  void update() override;

  void neutralBehavior();
  bool shouldProceed();
  void crashout();

  void normalProcedure();
  void stepForward();

  bool shouldCrashout();
  void crashoutProcedure();

  void draw(Vector2 &camera_target) override;
  void shakeEffect(Rectangle &dest);
  void drawDebug() override;

  PlayerCharacter *player;

  uint8_t cooldown_patience = 0;
private:
  uint8_t crashout_patience = 60;

  bool crashing_out = false;
  float crashout_time = 1.0;
  float crashout_timestamp = 0;

  float tick_timestamp = 0;

  std::vector<int> anim_walk;
  float walk_frametime;
  float run_frametime;

  std::vector<int> anim_death;
  float death_frametime;

  uint16_t player_dist;
  int16_t x_offset = 512;

  uint16_t preferred_dist;
  uint16_t crashout_dist;
  
  float step_distance;
};
