// sub-weapons/weapon_knife.cpp
#include <memory>
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_knife_light.h"
#include "weapon_knife.h"
#include <plog/Log.h>

using std::unique_ptr, std::make_unique;


WeaponKnife::WeaponKnife(PlayerCharacter *player) : 
  SubWeapon(player, "Knife", 5, 0)
{

}

unique_ptr<ActionCommand> WeaponKnife::lightTechnique() {
  bool sufficent_morale = player->morale >= mp_cost1;
  if (sufficent_morale) {
    player->morale -= mp_cost1;
    return make_unique<KnifeLight>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    return nullptr;
  }
}
