// base/sub-weapon.h
#pragma once
#include <memory>
#include <cstdint>
#include "generics.h"


class SubWeapon {
public:
  SubWeapon(PlayerCharacter *player, uint8_t mp_cost1, uint8_t mp_cost2);

  virtual void update() {}
  
  virtual std::unique_ptr<ActionCommand> lightTechnique();
  virtual std::unique_ptr<ActionCommand> heavyTechnique();

  bool usable = true;

  uint8_t mp_cost1;
  uint8_t mp_cost2;
protected:
  PlayerCharacter *player;
};
