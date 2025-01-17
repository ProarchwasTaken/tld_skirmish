// weapon_knife.h
#pragma once
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


/* The first SubWeapon to ever be implemented to the game. Which I'm
 * starting to notice is another recurring trend in these last 2 games,
 * but what can I say? This sub-weapon should serve as a basis for how
 * future sub-weapons should be programmed and implemented. Despite how 
 * the code looks, it's relatively straight-forward.*/
class WeaponKnife : public SubWeapon{
public:
  WeaponKnife(PlayerCharacter *player);

  void update() override;

  std::unique_ptr<ActionCommand> lightTechnique() override; 
  std::unique_ptr<ActionCommand> heavyTechnique() override;

  /* Out of the two handling functions, only this one will be properly
   * defined. Allows the player to cancel the attack into a Heavy Attack
   * or Technique if it Light Technique connects.*/
  void lightTechHandling() override;

private:
  float cooldown_time;
};
