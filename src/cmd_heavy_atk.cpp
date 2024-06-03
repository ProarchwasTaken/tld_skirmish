// cmd_heavy_atk.cpp
#include <raylib.h>
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_heavy_atk.h"


HeavyAttack::HeavyAttack(PlayerCharacter *player):
  ActionCommand(player, "Heavy Attack", 0.4, 0.2, 0.4)
{
  damage = 20;
  stun_time = 0.2;
  setupHurtbox();
}

void HeavyAttack::setupHurtbox() {
  float width = 48;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 48;

  hurtbox = {x, y, width, height};
}

void HeavyAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  enemyHitCheck();
}

void HeavyAttack::enemyHitCheck() {
  auto player = reinterpret_cast<PlayerCharacter*>(user);
  for (auto enemy : *player->enemies) {
    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, stun_time);
      player->state = RECOVER;
      return;
    }
  }
}

void HeavyAttack::draw() {
  if (user->state == ACT) {
    DrawRectangleRec(hurtbox, MAROON);
  } 
}

void HeavyAttack::drawDebug() {

}
