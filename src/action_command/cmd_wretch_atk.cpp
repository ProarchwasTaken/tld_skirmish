// action_command/cmd_wretch_atk.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include <triblib/utils/sound.h>
#include "combatants/enemy_wretch.h"
#include "action_command/cmd_wretch_atk.h"


WretchAttack::WretchAttack(WretchEnemy *user): 
  ActionCommand(user, "Wretch Attack", CMD_NONE, 0.4, 0.1, 0.8)
{
  damage = 12;
  guard_pierce = 0.45;

  stun_time = 0.75;

  this->player = user->player;
  user->current_sprite = sprites::wretch[9];
  setupHurtbox();

  SoundUtils::play("wrh_warning");
}

void WretchAttack::setupHurtbox() {
  float width = 64;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 48;

  hurtbox = {x, y, width, height};
}

void WretchAttack::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->current_sprite = sprites::wretch[10];
    SoundUtils::play("wrh_attack");
  }
}

void WretchAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);
  playerHitCheck();

  if (finished_action || attack_connected) {
    user->current_sprite = sprites::wretch[11];
  }
}

void WretchAttack::playerHitCheck() {
  if (player->state == DEAD) {
    return;
  }

  if (CheckCollisionRecs(hurtbox, player->hitbox)) {
    player->takeDamage(damage, guard_pierce, stun_time, 1.5, 
                       user->direction);

    attack_connected = true;
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void WretchAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  }
}
