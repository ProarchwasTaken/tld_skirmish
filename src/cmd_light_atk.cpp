// cmd_light_attack.cpp
#include <raylib.h>
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_light_atk.h"

LightAttack::LightAttack(Combatant &user) : 
  ActionCommand(user, "Light Attack", 0.1, 0.1, 0.2)
{
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

void LightAttack::draw() {
  if (user->state == ACT) {
    DrawRectangleRec(hurtbox, MAROON);
  } 
}

void LightAttack::drawDebug() {

}
