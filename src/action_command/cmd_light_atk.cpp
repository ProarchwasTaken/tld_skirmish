// action_command/cmd_light_atk.cpp
#include <raylib.h>
#include <raymath.h>
#include <triblib/base/generics.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "combatants/char_player.h"
#include "action_command/cmd_light_atk.h"


LightAttack::LightAttack(PlayerCharacter *user): 
  ActionCommand(user, "Light Attack", CMD_NORM_LIGHT, 0.2, 0.1, 0.3)
{
  damage = 3;
  guard_pierce = 0.2;

  stun_time = 0.5;

  this->enemies = user->enemies;
  this->player = user;
  user->current_sprite = sprites::player[4];

  if (player->moving) {
    decelerate = true;
  }
}

void LightAttack::setupHurtbox() {
  float width = 22;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 56;

  hurtbox = {x, y, width, height};
}

void LightAttack::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (decelerate) {
    const float max_speed = player->movement_speed;
    player->decelerate(percentage, charge_time, max_speed);
  }

  if (finished_charge) {
    user->current_sprite = sprites::player[5];
    setupHurtbox();
  }
}

void LightAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  enemyHitCheck(); 

  if (finished_action) {
    user->current_sprite = sprites::player[4];
  }
}

void LightAttack::enemyHitCheck() {
  for (auto enemy : *enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      if (enemy->isUsingCommand()) player->incrementMorale(1);

      enemy->takeDamage(damage, guard_pierce, stun_time, 0.15f, 
                        user->direction);

      attack_connected = true;
      break;
    }
  }

  if (attack_connected) {
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void LightAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}
