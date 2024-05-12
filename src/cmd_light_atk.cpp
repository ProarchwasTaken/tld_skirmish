// cmd_light_attack.cpp
#include <raylib.h>
#include "base/combatant.h"
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_light_atk.h"

LightAttack::LightAttack(Combatant &user) : 
  ActionCommand(user, "Light Attack", 0.1, 0.1, 0.2)
{
  damage = 10;
  stun_time = 0.2;
  setupHurtbox();
}

void LightAttack::setupHurtbox() {
  float width = 48;
  float half_width = width / 2;

  float height = 16;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 56;

  hurtbox = {x, y, width, height};
}

void LightAttack::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  if (user->type == TYPE_PLAYER) {
    auto player = reinterpret_cast<PlayerCharacter*>(user); 
    enemyHitCheck(*player);
  }
}

void LightAttack::enemyHitCheck(PlayerCharacter &player) {
  for (auto enemy : *player.enemies) {
    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, stun_time);
      player.state = RECOVER;
      return;
    }
  }
}

void LightAttack::draw() {
  if (user->state == ACT) {
    DrawRectangleRec(hurtbox, MAROON);
  } 
}

void LightAttack::drawDebug() {

}
