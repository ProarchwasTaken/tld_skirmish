// base/sub-weapon.cpp
#include <cstdint>
#include <memory>
#include <string>
#include <triblib/globals.h>
#include "combatants/char_player.h"
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include <plog/Log.h>

using std::unique_ptr, std::string;


SubWeapon::SubWeapon(PlayerCharacter *player, string name, 
                     uint8_t mp_cost1, uint8_t mp_cost2)
{
  this->player = player;
  this->name = name;

  this->mp_cost1 = mp_cost1;
  this->mp_cost2 = mp_cost2;

  disabled_timestamp = CURRENT_TIME;
  PLOGD << "Assigned sub-weapon: '" << name << "' to the player.";
}

SubWeapon::~SubWeapon() {
  PLOGD << name << " sub-weapon is cleared from memory.";
}

void SubWeapon::disable() {
  PLOGI << "Disabling: " << name;
  usable = false;
  disabled_timestamp = CURRENT_TIME;
}

unique_ptr<ActionCommand> SubWeapon::lightTechnique() {
  PLOGE << "This Method from: '" << name << "' is not overridden!";
  return nullptr;
}

unique_ptr<ActionCommand> SubWeapon::heavyTechnique() {
  PLOGE << "This Method from: '" << name << "' is not overridden!";
  return nullptr;
}


