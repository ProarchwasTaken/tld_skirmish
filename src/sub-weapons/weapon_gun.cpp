// sub-weapons/weapons_gun.cpp
#include <cstdint>
#include <memory>
#include "base/action_command.h"
#include "base/sub-weapon.h"
#include "globals.h"
#include "char_player.h"
#include "utils_sound.h"
#include "cmd_heavy_atk.h"
#include "cmd_gun_light.h"
#include "cmd_gun_heavy.h"
#include "weapon_gun.h"
#include <plog/Log.h>

using std::make_unique, std::unique_ptr;


WeaponGun::WeaponGun(PlayerCharacter *player) : 
  SubWeapon(player, "Stun Gun", 2, 4) 
{

}

void WeaponGun::update() {
  if (usable) {
    return;
  }

  float time_elapsed = CURRENT_TIME - disabled_timestamp;
  if (time_elapsed >= cooldown_time) {
    PLOGI << "Stun Gun is now off cooldown.";
    SoundUtils::play("weapon_ready");
    usable = true;
  }
}

unique_ptr<ActionCommand> WeaponGun::lightTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not useable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost1;
  if (sufficent_morale) {
    player->morale -= mp_cost1;
    return make_unique<GunLight>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}

unique_ptr<ActionCommand> WeaponGun::heavyTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not useable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost2;
  if (sufficent_morale) {
    player->morale -= mp_cost2;
    return make_unique<GunHeavy>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}

void WeaponGun::lightTechHandling() {
  uint8_t first_input = player->input_buffer.front();
  unique_ptr<ActionCommand> command;

  auto light_gun = static_cast<GunLight*>(player->current_command.get());
  if (light_gun->hit_enemy == false) {
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
