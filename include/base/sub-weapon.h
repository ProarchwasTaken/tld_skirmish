// base/sub-weapon.h
#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include <triblib/base/generics.h>


/* Sub-Weapons are meant to act as an extension to the player's core
 * moveset. Providing them with two special action commands that cost
 * Morale to use. The adequate terms for these types of action commands
 * would be Weapon Techniques, or just Techniques. The base class itself
 * mostly consists of methods that are required to be overriden by class
 * who derive from it.*/
class SubWeapon {
public:
  SubWeapon(PlayerCharacter *player, std::string name, uint8_t mp_cost1, 
            uint8_t mp_cost2);
  ~SubWeapon();

  virtual void update() {}

  /* The "usable" boolean is obvious in it's purpose. This method is for
   * setting that boolean to false, and setting a timestamp that could be
   * used later. The resposibility of setting the value back to true is 
   * usually left up to the derived class.*/
  void disable();
  
  /* Returns a pointer to a sub-weapon's associated action commands if
   * conditions were met. They are meant to overriden by the derived 
   * class. Otherwise, they would just return a null pointer. On a side
   * note, this is where the check to see if the player has sufficent
   * morale is often made.*/
  virtual std::unique_ptr<ActionCommand> lightTechnique(); 
  virtual std::unique_ptr<ActionCommand> heavyTechnique();

  /* These method act similarly to lightAttackHandling and 
   * heavyAttackHandling in terms of they function, but for the Light and
   * heavy techniques respectively. Overriding these methods is entirely
   * optional.*/
  virtual void lightTechHandling() {}
  virtual void heavyTechHandling() {}

  std::string name;
  bool usable = true;

  uint8_t mp_cost1;
  uint8_t mp_cost2;

  float disabled_timestamp = 0;
protected:
  PlayerCharacter *player;
};
