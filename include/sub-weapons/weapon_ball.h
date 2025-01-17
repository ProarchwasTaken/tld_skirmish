// weapon_ball.h
#pragma once
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


/* The second SubWeapon, which is drastically different than the first 
 * one, defines the concept that each Sub-Weapon may force the player to
 * play the game entirely differently. The Sub-Weapon could be
 * described as a basket ball which the player uses in it's techniques.*/
class WeaponBall : public SubWeapon {
public:
  WeaponBall(PlayerCharacter *player);

  void update() override;

  std::unique_ptr<ActionCommand> lightTechnique() override;
  std::unique_ptr<ActionCommand> heavyTechnique() override;
  void lightTechHandling() override;

private:
  float cooldown_time;
};
