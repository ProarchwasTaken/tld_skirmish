// cmd_heavy_atk.cpp
#include <raylib.h>
#include "globals.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_heavy_atk.h"


HeavyAttack::HeavyAttack(PlayerCharacter *player):
  ActionCommand(player, "Heavy Attack", 0.3, 0.1, 0.6)
{
  damage = 20;
  stun_time = 0.5;

  user->current_sprite = sprites::player[4];
  setupHurtbox();
}

void HeavyAttack::setupHurtbox() {
  float width = 26;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 54;

  hurtbox = {x, y, width, height};
}

void HeavyAttack::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->current_sprite = sprites::player[6];
  }
}

void HeavyAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  enemyHitCheck();

  if (finished_action) {
    user->current_sprite = sprites::player[4];
  }
}

void HeavyAttack::enemyHitCheck() {
  auto player = reinterpret_cast<PlayerCharacter*>(user);
  for (auto enemy : *player->enemies) {
    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, stun_time);
      attack_connected = true;

      player->state = RECOVER;
      return;
    }
  }
}

void HeavyAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}
