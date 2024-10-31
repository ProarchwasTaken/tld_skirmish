// weapon_gun.h
#pragma once
#include <memory>
#include "base/sub-weapon.h"
#include "char_player.h"


class WeaponGun : public SubWeapon {
public:
  WeaponGun(PlayerCharacter *player);
  std::unique_ptr<ActionCommand> lightTechnique() override;

  float cooldown_time = 0;
};
