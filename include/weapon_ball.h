// weapon_ball.h
#pragma once
#include "base/sub-weapon.h"
#include "char_player.h"


class WeaponBall : public SubWeapon {
public:
  WeaponBall(PlayerCharacter *player);
};
