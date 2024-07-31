// weapon_knife.h
#pragma once
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


class WeaponKnife : public SubWeapon{
public:
  WeaponKnife(PlayerCharacter *player);

  std::unique_ptr<ActionCommand> lightTechnique() override; 
  std::unique_ptr<ActionCommand> heavyTechnique() override;
  void lightTechHandling() override;
};
