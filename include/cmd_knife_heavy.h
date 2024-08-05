// cmd_knife_heavy.h
#pragma once
#include <raylib.h>
#include "base/generics.h"
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include "char_player.h"


/* Heavy Technique that's associated with the Knife SubWeapon. Upon
 * use, the player will throw a knife that inflicts damage to the first
 * enemy it hits. After which, the player's sub-weapon will be disabled
 * for a couple seconds for obvious reasons.*/
class KnifeHeavy : public ActionCommand {
public:
  KnifeHeavy(PlayerCharacter *player);

  void chargeSequence(float time_elapsed) override;
  void createProjectile();
private:
  combatant_list *enemies;
  SubWeapon *sub_weapon;
};
