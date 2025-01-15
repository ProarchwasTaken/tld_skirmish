// sub_weapons/weapon_ball.cpp
#include <memory>
#include <cstdint>
#include "globals.h"
#include "base/combatant.h"
#include "base/sub-weapon.h"
#include "base/action_command.h"
#include <triblib/utils/sound.h>
#include "char_player.h"
#include "cmd_ball_light.h"
#include "cmd_ball_heavy.h"
#include "cmd_heavy_atk.h"
#include "cmd_guard.h"
#include "weapon_ball.h"
#include <plog/Log.h>

using std::unique_ptr, std::make_unique;


WeaponBall::WeaponBall(PlayerCharacter *player) :
  SubWeapon(player, "Basket Ball", 5, 5) 
{
  cooldown_time = 2.0;
}

void WeaponBall::update() {
  if (usable) {
    return;
  }

  float time_elapsed = CURRENT_TIME - disabled_timestamp;
  if (player->state == NEUTRAL && time_elapsed >= cooldown_time) {
    PLOGI << "Basketball is now off cooldown.";
    SoundUtils::play("weapon_ready");
    usable = true;
  }
}

unique_ptr<ActionCommand> WeaponBall::lightTechnique() {
  if (usable == false) {
    PLOGI << "The sub-weapon is not usable at the moment.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }

  bool sufficent_morale = player->morale >= mp_cost1;
  if (sufficent_morale) {
    player->morale -= mp_cost1;
    return make_unique<BallLight>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}

unique_ptr<ActionCommand> WeaponBall::heavyTechnique() {
  bool sufficent_morale = player->morale >= mp_cost2;

  if (usable == false) {
    return make_unique<BallHeavy>(player);
  }
  else if (sufficent_morale) {
    player->morale -= mp_cost2;
    return make_unique<BallHeavy>(player);
  }
  else {
    PLOGI << "Player has insufficent morale.";
    SoundUtils::play("weapon_error");
    return nullptr;
  }
}

void WeaponBall::lightTechHandling() {
  uint8_t first_input = player->input_buffer.front();
  unique_ptr<ActionCommand> command;

  auto *light_ball = static_cast<BallLight*>(
    player->current_command.get()
  );

  if (player->state != ACT) {
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
    case BTN_GUARD: {
      command = make_unique<Guard>(player, sprites::plr_metadata, 
                                   PLR_BOUNDS, true);
      break;
    }
  }

  if (command != nullptr) {
    PLOGI << "Chaining command into: " << command->command_name;
    SoundUtils::stop("ball_light_juke");
    SoundUtils::play("cmd_cancel");

    player->useCommand(command);
    player->invulnerable = false;
  }
}
