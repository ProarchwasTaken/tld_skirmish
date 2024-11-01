// sub-weapons/weapons_gun.cpp
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "char_player.h"
#include "utils_sound.h"
#include "cmd_gun_light.h"
#include "weapon_gun.h"
#include <plog/Log.h>

using std::make_unique, std::unique_ptr;


WeaponGun::WeaponGun(PlayerCharacter *player) : 
  SubWeapon(player, "Stun Gun", 2, 0) 
{

}

unique_ptr<ActionCommand> WeaponGun::lightTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not useable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost1;
  if (sufficent_morale) {
    player->morale -= mp_cost1;
    return make_unique<GunLight>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}
