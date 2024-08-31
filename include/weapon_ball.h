// weapon_ball.h
#pragma once
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"


class WeaponBall : public SubWeapon {
public:
  WeaponBall(PlayerCharacter *player);

  std::unique_ptr<ActionCommand> lightTechnique() override;
  std::unique_ptr<ActionCommand> heavyTechnique() override;
  void lightTechHandling() override;
};
