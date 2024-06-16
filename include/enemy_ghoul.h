// enemy_ghoul.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>
#include "base/combatant.h"
#include "char_player.h"

#define GOL_HP 50
#define GOL_MIN_PATIENCE 1
#define GOL_MAX_PATIENCE 15

#define GOL_HITBOX_SCALE (Vector2){18, 56} 
#define GOL_HITBOX_OFFSET (Vector2){-9, -58}


class GhoulEnemy : public Combatant {
public:
  GhoulEnemy(PlayerCharacter &player, Vector2 position);

  void update(double &delta_time) override;
  void draw() override;
  void drawDebug() override;

  /* For the ghoul's neutral behavior, the direction they're facing
   * would automatically be updated to face the player. Only performing
   * an attack once the player is within their preferred distance.*/
  void neutralBehavior(double &delta_time);

  /* Patience is a value that the enemy uses to judge whether it should
   * act or not. This method is called once every frame of which certain
   * conditions are met, as it's resposible for ticking that counter down
   * 20 times per second.*/
  void tickPatience();

  /* The ghoul will play an animation upon death. When that animation is
   * over and a certain amount of time as passed, the enemy will await
   * deletion.*/
  void deathSequence();

  /* Moves the Ghoul Enemy in a specific direction while correcting their
   * hitbox and tex_rect. Called one every frame of which the player is 
   * outside their preferred distance.*/
  void movement(double &delta_time);

  PlayerCharacter *player;
private:
  float movement_speed;

  std::vector<int> anim_walk;
  float walk_frametime;

  std::vector<int> anim_death;
  float death_frametime;
  float death_time;

  uint16_t player_dist;
  uint16_t preferred_dist;

  uint8_t patience;

  float tick_time;
  float tick_timestamp = 0;
};
