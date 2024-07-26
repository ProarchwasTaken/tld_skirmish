// sub-weapons/weapon_knife.cpp
#include <memory>
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_knife_light.h"
#include "weapon_knife.h"

using std::unique_ptr, std::make_unique;


WeaponKnife::WeaponKnife(PlayerCharacter *player) : 
  SubWeapon(player, 5, 0)
{

}

unique_ptr<ActionCommand> WeaponKnife::lightTechnique() {
  return make_unique<KnifeLight>(player);
}
