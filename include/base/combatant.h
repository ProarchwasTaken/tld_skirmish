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

  std::string name;
  uint16_t max_health;
  uint8_t type;

  uint16_t health;
  uint8_t state;

  std::unique_ptr<ActionCommand> current_command;

};
