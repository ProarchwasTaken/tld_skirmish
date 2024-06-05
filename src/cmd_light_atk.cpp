// cmd_light_atk.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_light_atk.h"


LightAttack::LightAttack(PlayerCharacter *player) : 
  ActionCommand(player, "Light Attack", 0.2, 0.1, 0.3)
{
  damage = 10;
  stun_time = 0.5;

  user->current_sprite = sprites::player[4];
  setupHurtbox();
}

void LightAttack::setupHurtbox() {
  float width = 24;
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
  auto player = reinterpret_cast<PlayerCharacter*>(user);
  for (auto enemy : *player->enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, stun_time);
      attack_connected = true;

      player->state = RECOVER;
      return;
    }
  }
}

void LightAttack::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  } 
}
