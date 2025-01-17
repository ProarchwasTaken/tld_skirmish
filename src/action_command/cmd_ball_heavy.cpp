// action_command/cmd_ball_heavy.cpp
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include <triblib/utils/sound.h>
#include <triblib/utils/dynamic.h>
#include "base/action_command.h"
#include "base/combatant.h"
#include "projectiles/proj_ball.h"
#include "combatants/char_player.h"
#include "action_command/cmd_ball_heavy.h"


BallHeavy::BallHeavy(PlayerCharacter *player):
  ActionCommand(player, "Ball Heavy", CMD_TECH_HEAVY, 0.5, 0.1, 0.2)
{
  enemies = player->enemies;
  sub_weapon = player->sub_weapon.get();

  damage = 4;
  guard_pierce = 0.15;

  stun_time = 0.4;

  normal_proc = player->sub_weapon->usable;
  if (normal_proc) {
    user->current_sprite = sprites::player[24];
  }
  else {
    user->current_sprite = sprites::player[25];

    charge_time = 0.25;
    act_time = 0.1;
    recovery_time = 0.5;
  }

  setupHurtbox();
}

void BallHeavy::setupHurtbox() {
  float width = 29;
  float half_width = width / 2;

  float height = 24;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 25;

  hurtbox = {x, y, width, height};
}

void BallHeavy::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge != true) {
    return;
  }

  user->current_sprite = sprites::player[26];

  if (normal_proc) {
    createProjectile();

    sub_weapon->disable();
    SoundUtils::play("ball_heavy_kick");
  }
}

void BallHeavy::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  if (normal_proc == false) {
    enemyHitCheck();
  }

  if (finished_action) {
    user->current_sprite = sprites::player[27];
  }
}

void BallHeavy::enemyHitCheck() {
  for (auto enemy : *enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, guard_pierce, stun_time, 0.35f,
                        user->direction);

      attack_connected = true;
      break;
    }
  }

  if (attack_connected) {
    finished_action = true;
    user->state = RECOVER;
    sequence_timestamp = CURRENT_TIME;
  }
}

void BallHeavy::createProjectile() {
  Vector2 offset = {7.0f * user->direction, -8};
  Vector2 position = Vector2Add(user->position, offset);

  auto player = static_cast<PlayerCharacter*>(user);
  Dynamic::create<BallProjectile>(position, player, enemies);
}

void BallHeavy::drawDebug() {
  if (normal_proc == false && user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  }
}
