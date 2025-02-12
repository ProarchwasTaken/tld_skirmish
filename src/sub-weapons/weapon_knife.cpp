// sub-weapons/weapon_knife.cpp
#include <cstdint>
#include <memory>
#include <triblib/globals.h>
#include <triblib/utils/sound.h>
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include "combatants/char_player.h"
#include "action_command/cmd_knife_light.h"
#include "action_command/cmd_knife_heavy.h"
#include "action_command/cmd_heavy_atk.h"
#include "sub-weapons/weapon_knife.h"
#include <plog/Log.h>

using std::unique_ptr, std::make_unique;


WeaponKnife::WeaponKnife(PlayerCharacter *player) : 
  SubWeapon(player, "Knife", 3, 5)
{
  cooldown_time = 1.0;
}

void WeaponKnife::update() {
  if (usable) {
    return;
  }

  float time_elapsed = CURRENT_TIME - disabled_timestamp;
  if (time_elapsed >= cooldown_time) {
    PLOGI << "Knife is now off cooldown.";
    SoundUtils::play("weapon_ready");
    usable = true;
  }
}

unique_ptr<ActionCommand> WeaponKnife::lightTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not usable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost1;
  if (sufficent_morale) {
    player->morale -= mp_cost1;
    return make_unique<KnifeLight>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}

unique_ptr<ActionCommand> WeaponKnife::heavyTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not usable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost2;
  if (sufficent_morale) {
    player->morale -= mp_cost2;
    return make_unique<KnifeHeavy>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}

void WeaponKnife::lightTechHandling() {
  uint8_t first_input = player->input_buffer.front();
  unique_ptr<ActionCommand> command;

  auto light_knife = static_cast<KnifeLight*>(
    player->current_command.get()
  );

  if (light_knife->attack_connected == false) {
    return;
  }

  switch (first_input) {
    case BTN_HEAVY_ATK: {
      command = make_unique<HeavyAttack>(player);
      break;
    }
    case BTN_HEAVY_TECH: {
      command = heavyTechnique();
      break;
    }
  }

  if (command != nullptr) {
    PLOGI << "Chaining command into: " << command->command_name;
    SoundUtils::play("cmd_cancel");
    player->useCommand(command);
  }
}
