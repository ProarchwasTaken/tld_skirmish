// cmd_light_attack.cpp
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_light_atk.h"

LightAttack::LightAttack(Combatant &user) : 
  ActionCommand(user, "Light Attack", 0.1, 0.1, 0.2)
{

}

void LightAttack::draw() {

}

void LightAttack::drawDebug() {

}
