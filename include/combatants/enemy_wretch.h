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


/* The Wretch is probably the most intriguing enemy I've came up with so 
 * far. Her appearance could be described as a floating torso wielding a 
 * giant sword stained with blood. This enemy shares a couple 
 * similarities with the Ghoul, moving towards the player at a slow speed,
 * and attacking as soon as they are within range. A notable difference
 * is that they will retreat for a bit after doing so, before the cycle
 * starts again.
 *
 * The Wretch has a long range, and a short charge up time. The player
 * wouldn't be able to run at her without sustaining a large amount of 
 * damage and knockback. To balance this out, the enemy has a longer
 * recovery time than most action commands, and could effectively be
 * baited into whiffing their attack.*/
class WretchEnemy : public Combatant {
public:
  WretchEnemy(PlayerCharacter &player, Vector2 position);

  void update() override;
  void draw(Vector2 &camera_target) override;
  void drawDebug() override;

  /* The enemy's neutral behavior and animation majorly depends on 
   * whether or not they still have retreat_patience. If they do, they 
   * will move in the opposite direction of the player, refusing to 
   * attack.*/
  void neutralBehavior();

  /* Only called when the enemy's retreat_patience is at zero. This
   * is where checks of whether the player is within range are made,
   * attacking when conditions are met, and reseting their 
   * retreat_patience to a random value.*/
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
