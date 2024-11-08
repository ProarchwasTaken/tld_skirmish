// action_command/cmd_gun_heavy.cpp
#include <cstddef>
#include "globals.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "char_player.h"
#include "cmd_gun_heavy.h"

GunHeavy::GunHeavy(PlayerCharacter *player): 
  ActionCommand(player, "Gun Heavy", CMD_TECH_HEAVY, 0.4, 0.0, 0.7)
{
  this->player = player;
  this->sub_weapon = player->sub_weapon.get();

  this->enemies = player->enemies;

  player->current_sprite = sprites::player[33];
  player->frame_timestamp = CURRENT_TIME;
}

GunHeavy::~GunHeavy() {
  player->current_anim = NULL;
}

void GunHeavy::chargeSequence(float time_elapsed) {
  if (input_released == false && techInputHeldDown()) {
    Animation::play(player, sprites::player, anim_charge,
                    flash_interval);
    sequence_timestamp = CURRENT_TIME;
    return;
  }
  else {
    ActionCommand::chargeSequence(time_elapsed); 
    input_released = true;
  }


  Animation::play(player, sprites::player, anim_shoot, shoot_frametime,
                  false);

  if (finished_charge) {
    player->current_sprite = sprites::player[41];
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
