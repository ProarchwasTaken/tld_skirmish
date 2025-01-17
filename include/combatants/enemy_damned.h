// enemy_damned.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/combatant.h"
#include "char_player.h"

#define DAM_HP 20
#define DAM_STABILITY 0.50

#define DAM_HIT_SCALE (Vector2){18, 56}
#define DAM_HIT_OFFSET (Vector2){-9, -58}

#define DAM_TEX_SCALE (Vector2){96, 64}
#define DAM_TEX_OFFSET (Vector2){-48, -64}


/* The Damned is by far the most "complicated" enemy while also being
 * relatively simple upon first glance. In terms of programming, I feel it
 * really stretches this game's systems to it's limits, and it's the 
 * culmination of everything I've done so far.
 *
 * The Damned will slowly step towards the player, with each step playing
 * a sound to alert the player of their presence. Once the player is
 * within range, the enemy will attempt to "grab" them. If the player
 * moves away from the enemy for too long, they will proceed to 
 * "crashout".*/
class DamnedEnemy : public Combatant {
public:
  DamnedEnemy(PlayerCharacter &player, Vector2 position);
  void update() override;

  /* The root method for the majority of the code for the enemy's 
   * behavior. The Damned behavior while in the NEUTRAL state is 
   * determined by two patience values rather than the standard one.*/
  void neutralBehavior();

  /* For checking if the enemy should proceed with it's neutral behavior,
   * being the first check that is made before doing anything else.*/
  bool shouldProceed();

  /* A safe way of updating the Damned's direction depending on the
   * difference between the player, and the enemy on the X axis. Primary
   * used for avoiding the edge case of the enemy's direction being set
   * to zero in places where that would be detrimental.*/
  void updateDirection(const float x_offset);

  /* The default behavior that the Damned uses. Basically involves slowly
   * stepping towards the player at a set interval that syncs with the
   * enemy's walking animation.*/
  void normalProcedure();

  /* Instantly changes the enemy's position on the X axis by a 
   * predetermined distance and direction. This method is NOT meant to 
   * be called every frame.*/
  void stepForward();

  /* Basically assigns the enemy with their associated ActionCommand.
   * May play a different sound if the enemy is crashing out at the time
   * of this being called. You've seen this before. This method is
   * primarily made to avoid reusing code.*/
  void attemptGrab();

  /* The series of conditions that must be met in order for the 
   * DamnedEnemy to crash out. With the predominate factor being the
   * enemy's "crashout_patience".*/
  bool shouldCrashout();
  void crashout();

  /* The enemy's behavior while they're crashing out. Makes the enemy
   * step towards the player at a much faster pace of which outrunning
   * is impossible. Only attempting a grab upon passing the player.
   * After which, they will return to normal behavior.*/
  void crashoutProcedure();

  /* Overidden for the sole purpose of reward the player's morale
   * for each time the method has been called while they are crashing
   * out.*/
  void takeDamage(uint16_t dmg_magnitude, float guard_pierce, 
                  float stun_time, float kb_velocity = 0, 
                  int8_t kb_direction = 0) override;

  void draw(Vector2 &camera_target) override;

  /* For apply a shake effect while the enemy has the player in their
   * grasp.*/
  void shakeEffect(Rectangle &dest);

  void drawDebug() override;

  PlayerCharacter *player;

  bool crashing_out = false;
  uint8_t cooldown_patience = 0;
private:
  uint8_t crashout_patience = 20;

  float crashout_time = 1.0;
  float crashout_timestamp = 0;

  float tick_timestamp = 0;

  std::vector<int> anim_walk;
  float walk_frametime;

  std::vector<int> anim_run;
  float run_frametime;

  std::vector<int> anim_death;
  float death_frametime;

  uint16_t player_dist;
  int16_t x_offset = 512;

  uint16_t preferred_dist;
  uint16_t crashout_dist;
  
  float step_distance;
};
