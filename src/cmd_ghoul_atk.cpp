// cmd_ghoul_atk.cpp
#include <raylib.h>
#include "base/generics.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "enemy_ghoul.h"
#include "cmd_ghoul_atk.h"


GhoulAttack::GhoulAttack(GhoulEnemy *user):
  ActionCommand(user, "Ghoul Attack", 0.3, 0.1, 0.4)
{
  damage = 30;
  stun_time = 0.5;

  this->player = user->player;
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

void GhoulAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);
  playerHitCheck();
}

void GhoulAttack::playerHitCheck() {
  if (player->state == DEAD) {
    return;
  }

  if (CheckCollisionRecs(hurtbox, player->hitbox)) {
    player->takeDamage(damage, stun_time);

    user->state = RECOVER;
    return;
  }
}

void GhoulAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}

