// base/sub-weapon.h
#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include "generics.h"


class SubWeapon {
public:
  SubWeapon(PlayerCharacter *player, std::string name, uint8_t mp_cost1, 
            uint8_t mp_cost2);

  virtual void update() {}
  
  virtual std::unique_ptr<ActionCommand> lightTechnique(); 
  virtual void lightTechHandling() {}

  virtual std::unique_ptr<ActionCommand> heavyTechnique();
  virtual void heavyTechHandling() {}

  std::string name;
  bool usable = true;

  uint8_t mp_cost1;
  uint8_t mp_cost2;
protected:
  PlayerCharacter *player;
};
