// action_command/cmd_knife_light.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_knife_light.h"


KnifeLight::KnifeLight(PlayerCharacter *user):
ActionCommand(user, "Knife Light", 0.2, 0.1, 0.3)
{
  damage = 6;
  guard_pierce = 3.0;

  stun_time = 0.5;

  this->enemies = user->enemies;
  user->current_sprite = sprites::player[10]; 
  setupHurtbox();
}

void KnifeLight::setupHurtbox() { 
  float width = 32;
  float half_width = width / 2;

  float height = 24;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 52;

  hurtbox = {x, y, width, height};
}

void KnifeLight::chargeSequence(float time_elapsed, double &delta_time) {
  ActionCommand::chargeSequence(time_elapsed, delta_time);

  if (finished_charge) {
    user->current_sprite = sprites::player[11];
  }
}

void KnifeLight::actSequence(float time_elapsed, double &delta_time) {
  ActionCommand::actSequence(time_elapsed, delta_time);

  enemyHitCheck();

  if (finished_action) {
    user->current_sprite = sprites::player[12];
  }
}

void KnifeLight::enemyHitCheck() {
  for (auto enemy : *enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, guard_pierce, stun_time, 0.1f, 
                        user->direction);
      attack_connected = true;
    }
  } 

  if (attack_connected) {
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void KnifeLight::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}
