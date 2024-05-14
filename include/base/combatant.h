// combatant.h
#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <raylib.h>
#include "actor.h"
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


/* Everything, including the player, enemies, and bosses derive from this
 * class. Sets the ground work for the "fighting" aspect of the game,
 * (Which is basically all of it.) so the implementation must be done
 * correctly. Every combatant will have a name and type that will be used
 * for identification.*/
class Combatant : public Actor {
public:
  Combatant(std::string name, uint8_t type, uint16_t max_health,
            Vector2 position, Vector2 hitbox_scale = {32, 64},
            Vector2 tex_scale = {64, 64});

  /* Called once every frame. Holds code for a combatant's unique logic,
   * and checks. So expect this to be overridded by derived classes.*/
  virtual void update(double &delta_time) {};

  /* Takes a action command as a parameter, and assigns it to the 
   * combatant. Acts as a streamlined way to assign any class that derives
   * from ActionCommand.*/
  virtual void useCommand(std::unique_ptr<ActionCommand> &command);

  /* For handling the all of the stages of using a command, like the 
   * charge up, action, and recovery. Is called once every frame of which
   * the combatant is in a state other than Neutral or Hit Stun.*/
  void commandSequence();

  /* Usually called after the combatant takes damage. Unless they are in
   * hit stun, they will be set back to neutral.*/
  void cancelCommand();

  /* For decrementing a combatant's health by a set amount while also
   * putting them in hit stun. Also makes sure the combatant's health will
   * not be below 0.*/
  void takeDamage(uint16_t damage_magnitude, float stun_time);

  /* Called once every frame of which the combatant is in hit stun. If a
   * certain amount of time as passed and the combatant didn't take 
   * damage during that time, then their state will be set back to 
   * neutral.*/
  void stunSequence();

  std::string name;
  uint16_t max_health;
  uint8_t type;

  uint16_t health;
  uint8_t state;
  int8_t direction;

  std::unique_ptr<ActionCommand> current_command;
protected:
  float stun_time = 0;
  float stun_timestamp = 0;
};
