// base/sub-weapon.cpp
#include <cstdint>
#include "char_player.h"
#include "base/sub-weapon.h"


SubWeapon::SubWeapon(PlayerCharacter &player, uint8_t mp_cost1, 
                     uint8_t mp_cost2)
{
  this->player = &player;

  this->mp_cost1 = mp_cost1;
  this->mp_cost2 = mp_cost2;
}
