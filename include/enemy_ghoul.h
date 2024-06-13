// enemy_ghoul.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>
#include "base/combatant.h"
#include "char_player.h"

#define GOL_HP 50
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

  /* Moves the Ghoul Enemy in a specific direction while correcting their
   * hitbox and tex_rect. Called one every frame of which the player is 
   * outside their preferred distance.*/
  void movement(double &delta_time);

  PlayerCharacter *player;
private:
  float movement_speed;

  std::vector<int> anim_walk;
  float walk_frametime;

  uint16_t player_dist;
  uint16_t preferred_dist;
};
