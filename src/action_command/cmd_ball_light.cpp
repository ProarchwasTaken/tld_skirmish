// action_command/cmd_ball_light.cpp
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "utils_sound.h"
#include "char_player.h"
#include "cmd_ball_light.h"


BallLight::BallLight(PlayerCharacter *user):
  ActionCommand(user, "Ball Light", CMD_TECH_LIGHT, 0.1,  0.8, 0.6)
{
  anim_juke = {20, 21, 22, 21};
  juke_frametime = 0.15;

  player = user;
  player->current_sprite = sprites::player[19];
}

BallLight::~BallLight() {
  // PS: It's very important to set the player's current animation to NULL
  // after this is used. Not gonna sugarcoat it: segfaults.
  player->current_anim = NULL;
}

void BallLight::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  if (finished_charge) {
    user->invulnerable = true;
    SoundUtils::play("ball_light_juke");
  }
}

void BallLight::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  Animation::play(user, sprites::player, anim_juke, juke_frametime);
  player->updateDirection();
  player->movement(player->movement_speed * 2, true);

  if (finished_action) {
    player->current_sprite = sprites::player[23];
    player->invulnerable = false;
    SoundUtils::play("ball_light_break");
  }
}

void BallLight::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);

  float max_speed = player->movement_speed * 2;
  float speed = Lerp(0, max_speed, speed_percentage);
  player->movement(speed, true);

  speed_percentage -= GetFrameTime() / recovery_time;
  speed_percentage = Clamp(speed_percentage, 0.0, max_speed);
}
