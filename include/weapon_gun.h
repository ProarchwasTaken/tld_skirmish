// weapon_gun.h
#pragma once
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


class WeaponGun : public SubWeapon {
public:
  WeaponGun(PlayerCharacter *player);

  void update() override;

  std::unique_ptr<ActionCommand> lightTechnique() override;
  std::unique_ptr<ActionCommand> heavyTechnique() override;
  void lightTechHandling() override;

  float cooldown_time = 2;
};
