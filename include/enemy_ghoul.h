// enemy_ghoul.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>
#include "base/combatant.h"
#include "char_player.h"

#define GOL_HP 15
#define GOL_STABILITY 0.35
#define GOL_MIN_PATIENCE 1
#define GOL_MAX_PATIENCE 8

#define GOL_HITBOX_SCALE (Vector2){18, 56} 
#define GOL_HITBOX_OFFSET (Vector2){-9, -58}


/* The Ghoul enemy is supposed to set the baseline for every other enemy
 * in the game. It will move towards the player at a slow speed and 
 * performs an attack when close enough.*/
class GhoulEnemy : public Combatant {
public:
  GhoulEnemy(PlayerCharacter &player, Vector2 position);

  void update() override;
  void draw(Vector2 &camera_target) override;
  void drawDebug() override;

  /* For the ghoul's neutral behavior, the direction they're facing
   * would automatically be updated to face the player. Only performing
   * an attack once the player is within their preferred distance.*/
  void neutralBehavior();

  /* Moves the Ghoul Enemy in a specific direction while correcting their
   * hitbox and tex_rect. Called one every frame of which the player is 
   * outside their preferred distance.*/
  void movement();

  PlayerCharacter *player;
private:
  float movement_speed;

  std::vector<int> anim_walk;
  float walk_frametime;

  std::vector<int> anim_death;
  float death_frametime;

  uint16_t player_dist;
  uint16_t preferred_dist;

  uint8_t attack_patience;
  float tick_timestamp = 0;
};
