// proj_ball.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/generics.h"
#include "base/dynamic_actor.h"


class BallProjectile : public DynamicActor {
public:
  BallProjectile(Vector2 position, PlayerCharacter *player, 
                 combatant_list *enemies);

  void update() override;
  void draw(Vector2 &camera_target) override;

  void movement();
  void enemyHitCheck();
  void playerKickCheck();
private:
  PlayerCharacter *player;
  combatant_list *enemies;

  int8_t direction; 

  bool kickable = false;
  bool can_bounce = true;

  uint8_t bounce_count = 0;
  uint8_t bounce_limit;

  uint16_t damage;
  float velocity;

  float sound_pitch = 1.0;

  float stun_time;
  float guard_pierce;

  std::vector<int> anim_spin;
  float spin_frametime;
};
