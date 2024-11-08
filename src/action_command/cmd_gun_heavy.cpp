// action_command/cmd_gun_heavy.cpp
#include <cstddef>
#include "globals.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "char_player.h"
#include "cmd_gun_heavy.h"
#include <plog/Log.h>


GunHeavy::GunHeavy(PlayerCharacter *player): 
  ActionCommand(player, "Gun Heavy", CMD_TECH_HEAVY, 0.4, 0.0, 0.7)
{
  this->player = player;
  this->sub_weapon = player->sub_weapon.get();

  this->enemies = player->enemies;

  player->current_sprite = sprites::player[33];
  player->frame_timestamp = CURRENT_TIME;
  level_timestamp = CURRENT_TIME;
}

GunHeavy::~GunHeavy() {
  player->current_anim = NULL;
}

void GunHeavy::chargeSequence(float time_elapsed) {
  if (input_released == false && techInputHeldDown()) {
    chargeAnimation();
    charge();
    sequence_timestamp = CURRENT_TIME;
    return;
  }
  else {
    ActionCommand::chargeSequence(time_elapsed); 
    input_released = true;
  }

  shootAnimation();
  if (finished_charge) {
    player->current_sprite = sprites::player[41];
  }
}

void GunHeavy::chargeAnimation() {
  if (level != max_level) {
    Animation::play(player, sprites::player, anim_charge,
                    charge_frametime);
  }
  else {
    Animation::play(player, sprites::player, anim_altcharge,
                    charge_frametime); 
  }
}

void GunHeavy::charge() {
  if (level == max_level) {
    return;
  }

  float time_elapsed = CURRENT_TIME - level_timestamp;
  if (time_elapsed >= level_interval) {
    level++;
    level_timestamp = CURRENT_TIME;
  }
}

void GunHeavy::shootAnimation() {
  if (level != max_level) {
    Animation::play(player, sprites::player, anim_shoot, shoot_frametime,
                    false);
  }
  else {
    Animation::play(player, sprites::player, anim_altshoot, 
                    shoot_frametime, false);
  }
}

bool GunHeavy::techInputHeldDown() {
  bool key_s = IsKeyDown(KEY_S);
  bool btn_face_up = false;

  if (IsGamepadAvailable(0)) {
    btn_face_up = IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP);
  }

  return key_s || btn_face_up;
}
