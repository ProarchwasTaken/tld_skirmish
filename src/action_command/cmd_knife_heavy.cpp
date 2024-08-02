// action_command/cmd_knife_heavy.h
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/action_command.h"
#include "utils.h"
#include "proj_knife.h"
#include "char_player.h"
#include "cmd_knife_heavy.h"


KnifeHeavy::KnifeHeavy(PlayerCharacter *player):
  ActionCommand(player, "Knife Heavy", CMD_TECH_HEAVY, 0.25, 0.1, 0.0) 
{
  enemies = player->enemies;
  sub_weapon = player->sub_weapon.get();

  user->current_sprite = sprites::player[13];
}

void KnifeHeavy::chargeSequence(float time_elapsed, double &delta_time) {
  ActionCommand::chargeSequence(time_elapsed, delta_time);

  if (finished_charge) {
    user->current_sprite = sprites::player[14];

    SoundUtils::play("knife_heavy");
    createProjectile();

    sub_weapon->disable();
  }
}

void KnifeHeavy::createProjectile() {
  Vector2 offset = {20.0f * user->direction, -48.0};
  Vector2 position = Vector2Add(user->position, offset);

  Dynamic::create<KnifeProjectile>(position, user->direction, enemies);
}
