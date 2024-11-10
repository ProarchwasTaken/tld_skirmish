// action_command/cmd_gun_heavy.cpp
#include <raylib.h>
#include <cstdint>
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
  drain_timestamp = CURRENT_TIME;
}

GunHeavy::~GunHeavy() {
  player->current_anim = NULL;
}

void GunHeavy::chargeSequence(float time_elapsed) {
  if (input_released == false && techInputHeldDown()) {
    chargeAnimation();
    charge();
    moraleDrain();

    sequence_timestamp = CURRENT_TIME;
    return;
  }
  else {
    ActionCommand::chargeSequence(time_elapsed); 

    float offset = 32 * player->direction;
    player->camera_position = player->position.x + offset;
    input_released = true;
  }

  shootAnimation();
  if (finished_charge) {
    shoot();
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

void GunHeavy::moraleDrain() {
  float time_elapsed = CURRENT_TIME - drain_timestamp;

  if (time_elapsed < drain_time) {
    return;
  }

  if (player->morale != 0) {
    player->morale--;
    drain_timestamp = CURRENT_TIME;
  }
  else {
    input_released = true;
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

Rectangle GunHeavy::setupHurtbox() { 
  float width = min_range + (32 * level);
  float half_width = width / 2;

  float height = 4;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 52;

  Rectangle hurtbox = {x, y, width, height};
  return hurtbox;
}

void GunHeavy::shoot() {
  Rectangle hurtbox = setupHurtbox();
  uint8_t damage = min_damage + (2 * level);
  float stun_time = min_stuntime + (0.25 * level);

  for (auto enemy : *enemies) {
    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      enemy->takeDamage(damage, 0.25, stun_time);
    }
  }
}

void GunHeavy::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);

  if (finished_recovering) {
    player->camera_position = player->position.x;
  }
}
