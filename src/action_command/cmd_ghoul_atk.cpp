// action_command/cmd_ghoul_atk.cpp
#include <raylib.h>
#include <triblib/base/generics.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "combatants/enemy_ghoul.h"
#include "action_command/cmd_ghoul_atk.h"


GhoulAttack::GhoulAttack(GhoulEnemy *user):
  ActionCommand(user, "Ghoul Attack", CMD_NONE, 0.3, 0.1, 0.4)
{
  damage = 8;
  guard_pierce = 0.4;

  stun_time = 0.5;

  this->player = user->player;
  user->current_sprite = sprites::ghoul[4];
  setupHurtbox();
}

void GhoulAttack::setupHurtbox() {
  float width = 24;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 56;

  hurtbox = {x, y, width, height};
}

void GhoulAttack::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->current_sprite = sprites::ghoul[5];
  }
}

void GhoulAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);
  playerHitCheck();

  if (finished_action || attack_connected) {
    user->current_sprite = sprites::ghoul[6];
  }
}

void GhoulAttack::playerHitCheck() {
  if (player->state == DEAD) {
    return;
  }

  if (CheckCollisionRecs(hurtbox, player->hitbox)) {
    player->takeDamage(damage, guard_pierce, stun_time, 1, 
                       user->direction);

    attack_connected = true;
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void GhoulAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}

