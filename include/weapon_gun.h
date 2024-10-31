// weapon_gun.h
#pragma once
#include "base/sub-weapon.h"
#include "char_player.h"


class WeaponGun : public SubWeapon {
public:
  WeaponGun(PlayerCharacter *player);

  float cooldown_time = 0;
};
