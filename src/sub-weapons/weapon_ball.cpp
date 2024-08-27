// sub_weapons/weapon_ball.cpp
#include <memory>
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include "utils.h"
#include "char_player.h"
#include "cmd_ball_light.h"
#include "weapon_ball.h"
#include <plog/Log.h>

using std::unique_ptr, std::make_unique;


WeaponBall::WeaponBall(PlayerCharacter *player) :
  SubWeapon(player, "Basket Ball", 0, 0) 
{

}

unique_ptr<ActionCommand> WeaponBall::lightTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not usable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost1;
  if (sufficent_morale) {
    player->morale -= mp_cost1;
    return make_unique<BallLight>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}
