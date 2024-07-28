// action_command/cmd_heavy_atk.cpp
#include <raylib.h>
#include "globals.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_heavy_atk.h"


HeavyAttack::HeavyAttack(PlayerCharacter *user):
  ActionCommand(user, "Heavy Attack", CMD_NORM_HEAVY, 0.3, 0.1, 0.6)
{
  damage = 5;
  guard_pierce = 0.25;

  stun_time = 0.5;

  this->enemies = user->enemies;
  this->player = user;
  user->current_sprite = sprites::player[4];
  setupHurtbox();
}

void HeavyAttack::setupHurtbox() {
  float width = 24;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 54;

  hurtbox = {x, y, width, height};
}

void HeavyAttack::chargeSequence(float time_elapsed, double &delta_time) {
  ActionCommand::chargeSequence(time_elapsed, delta_time);

  if (finished_charge) {
    user->current_sprite = sprites::player[6];
  }
}

void HeavyAttack::actSequence(float time_elapsed, double &delta_time) {
  ActionCommand::actSequence(time_elapsed, delta_time);

  enemyHitCheck();

  if (finished_action) {
    user->current_sprite = sprites::player[4];
  }
}

void HeavyAttack::enemyHitCheck() {
  bool morale_eligible = false;

  for (auto enemy : *enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      if (enemy->isUsingCommand()) morale_eligible = true;

      enemy->takeDamage(damage, guard_pierce, stun_time, 0.5f, 
                        user->direction);

      attack_connected = true;
    }
  }

  if (morale_eligible) {
    player->incrementMorale(1);
  }
  if (attack_connected) {
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void HeavyAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}
