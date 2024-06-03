// cmd_light_atk.cpp
#include <raylib.h>
#include "base/action_command.h"
#include "char_player.h"
#include "cmd_light_atk.h"


LightAttack::LightAttack(PlayerCharacter *player) : 
  ActionCommand(player, "Light Attack", 0.2, 0.1, 0.2)
{
  damage = 10;
  stun_time = 0.1;
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

  enemyHitCheck(); 
}

void LightAttack::enemyHitCheck() {
  auto player = reinterpret_cast<PlayerCharacter*>(user);
  for (auto enemy : *player->enemies) {
    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, stun_time);
      player->state = RECOVER;
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
