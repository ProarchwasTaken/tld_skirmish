// weapon_gun.h
#pragma once
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


/* The third and final SubWeapon. It could be described as a stun gun 
 * that's been modified to be a lot more lethal. Each of the sub-weapon's
 * techniques involve holding the button used to perform them for better
 * effect.*/
class WeaponGun : public SubWeapon {
public:
  WeaponGun(PlayerCharacter *player);

  void update() override;

  std::unique_ptr<ActionCommand> lightTechnique() override;
  std::unique_ptr<ActionCommand> heavyTechnique() override;
  void lightTechHandling() override;

  float cooldown_time = 2;
};
