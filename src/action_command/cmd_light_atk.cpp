// action_command/cmd_light_atk.cpp
#include <raylib.h>
#include "globals.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_light_atk.h"


LightAttack::LightAttack(PlayerCharacter *user): 
  ActionCommand(user, "Light Attack", 0.2, 0.1, 0.3)
{
  damage = 10;
  guard_pierce = 0.2;

  stun_time = 0.5;

  this->enemies = user->enemies;
  user->current_sprite = sprites::player[4];
  setupHurtbox();
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

  if (finished_charge) {
    user->current_sprite = sprites::player[5];
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
      enemy->takeDamage(damage, guard_pierce, stun_time, 0.15f, 
                        user->direction);
      attack_connected = true;

      user->state = RECOVER;
      return;
    }
  }
}

void LightAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}
