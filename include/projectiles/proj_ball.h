// proj_ball.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include <triblib/base/generics.h>
#include <triblib/base/dynamic_actor.h>


/* DynamicActor created when the player successfully uses BallHeavy while
 * their sub-weapon is enabled. It moves in the direction the player has
 * kicked it, and bounces off the first enemy it hits, inflicting damage.
 * After that the player could kick it back. which increases it's speed
 * and damage potential. This could only be done a number of times before
 * the ball won't bounce off enemies anymore, and just pierces through
 * them.*/
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
