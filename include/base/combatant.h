// combatant.h
#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <raylib.h>
#include <vector>
#include <triblib/base/actor.h>
#include "action_command.h"

#define TYPE_PLAYER 0
#define TYPE_ENEMY 1

#define RIGHT 1
#define LEFT -1

#define NEUTRAL 0
#define CHARGING 1
#define ACT 2
#define RECOVER 3
#define HIT_STUN 4
#define DEAD 5


/* Everything, including the player, enemies, and bosses derive from this
 * class. Sets the ground work for the "fighting" aspect of the game,
 * (Which is basically all of it.) so the implementation must be done
 * correctly. Every combatant will have a name and type that will be used
 * for identification.*/
class Combatant : public Actor {
public:
  Combatant(std::string name, uint8_t type, uint16_t max_health,
            float stability, Vector2 position, 
            Vector2 hitbox_scale = {32, 64}, Vector2 tex_scale = {64, 64}, 
            Vector2 hitbox_offset = {-16, -64}, 
            Vector2 tex_offset = {-32, -64});

  /* Called once every frame. Holds code for a combatant's unique logic,
   * and checks. So expect this to be overridded by derived classes.*/
  virtual void update() {};

  /* Takes a action command as a parameter, and assigns it to the 
   * combatant. Acts as a streamlined way to assign any class that derives
   * from ActionCommand.*/
  void useCommand(std::unique_ptr<ActionCommand> &command);

  /* For handling the all of the stages of using a command, like the 
   * charge up, action, and recovery. Is called once every frame of which
   * the combatant is in a state other than Neutral or Hit Stun.*/
  void commandSequence();

  /* Returns true if combatant is currently using an action command if 
   * haven't already figured it out.*/
  bool isUsingCommand();

  /* Usually called after the combatant takes damage. Unless they are in
   * hit stun, they will be set back to neutral.*/
  void cancelCommand();

  /* For decrementing a combatant's health by a set amount while also
   * putting them in hit stun. Also makes sure the combatant's health will
   * not be below 0. If the stun_time parameter is 0, the combatant
   * will not be put into hit stun, and a death check will be made.*/
  virtual void takeDamage(uint16_t dmg_magnitude, float guard_pierce, 
                          float stun_time, float kb_velocity = 0, 
                          int8_t kb_direction = 0);

  /* Acts as straight forward wrapper for creating a instance of Damage
   * Number, and positioning it right above the combatant's tex_rect.
   * Typically called after the combatant takes damage.*/
  void createDamageNumber(int value, Color color);

  /* Causes the combatant to enter the hit stun sequence. Automatically
   * cancelling any action command the combatant was performing at the
   * time the method was called.*/
  void enterHitStun(float stun_time);

  /* What a function named "setKnockback" would do is beyond me. An 
   * absolute mystery. To be real, this function only updates the
   * knockback variables when certain conditions apply.*/
  void setKnockback(float kb_velocity, int8_t kb_direction, 
                    bool force = false);

  /* For applying knockback primarily, during the stun sequence. Also
   * makes sure that the combatant won't move past any boundaries.*/
  void applyKnockback(uint16_t boundary);

  /* Every now and then, the combatant makes a check to see if they are 
   * legally considered dead. If so, this method is called to 
   * "Seal the deal" so to speak. The death check is usually made after 
   * the combatant exits HIT_STUN, or right after they received damage 
   * that doesn't cause them to get sent into HIT_STUN. What happens 
   * after the combatant dies is completely up to the class that 
   * inherits from it.*/
  void death();

  /* Called once every frame of which the combatant is in hit stun. If a
   * certain amount of time has passed and the combatant didn't take 
   * damage that doesn't cause their stun_timestamp to reset, they will
   * automatically be taken out of hitstun.*/
  void stunSequence();

  /* Recommended method for taking a combatant out of hit stun. Resets
   * specific variables back to their default values, and it's one of
   * a few places where the death check is made*/
  void endStunSequence();

  /* For slowly regenerating a combatant's stability back to full at the
   * same rate of which an enemy's patience would tick down. The game use
   * case for this method is when the Combatant is in the NEUTRAL state.*/
  void stabilityRegen();

  /* A sort of wrapper for Animation::play. A common method used by those
   * deriving from combatant to play death animations. Automatically 
   * deleting themselves once they've reached the end of that animation.*/
  void deathSequence(std::vector<Texture*> &sprite_list, 
                     std::vector<int> &anim_death,
                     float death_frametime);

  std::string name;
  uint16_t max_health;
  float max_stability;
  uint8_t type;

  bool awaiting_deletion = false;
  bool invulnerable = false;

  uint16_t health;
  uint8_t state;
  int8_t direction;

  float stability;
  bool parried_attack = false;
  uint16_t combo = 0;

  std::unique_ptr<ActionCommand> current_command;
protected:
  float stun_time = 0;
  float stun_timestamp = 0;

  float stability_timestamp = 0;

  int8_t kb_direction = 0;
  float kb_velocity = 0;

  float death_timestamp;
};
