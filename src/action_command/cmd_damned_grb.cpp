// action_command/cmd_damned_grb.cpp
#include <cstddef>
#include <raylib.h>
#include <random>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "enemy_damned.h"
#include "cmd_damned_grb.h"

using std::uniform_int_distribution;


DamnedGrab::DamnedGrab(DamnedEnemy *user):
  ActionCommand(user, "Damned Grab", CMD_NONE, 0.8, 0.1, 0.5) 
{
  this->player = user->player;

  // This is here so it would be consistant across all FPS ranges.
  stun_time = (grab_time + 0.5) * DELTA_TIME;
}

DamnedGrab::~DamnedGrab() {
  user->current_anim = NULL;

  if (player->visible == false) {
    player->visible = true;
  }
}

void DamnedGrab::setupHurtbox() {
  float width = 35;
  float half_width = width / 2;

  float height = 32;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 56;

  hurtbox = {x, y, width, height};
}

void DamnedGrab::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  Animation::play(this->user, sprites::damned, anim_charge, 
                  charge_frametime);

  if (finished_charge) {
    user->current_sprite = sprites::damned[8];
    setupHurtbox();
  }
}

void DamnedGrab::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  playerHitCheck();

  if (grabbed_player) {
    // I'm starting to think this line is more error prone than I
    // expected.
    player->visible = false;

    user->current_sprite = sprites::damned[10];
    recovery_time = grab_time;
  }
  else if (finished_action || player->parried_attack) {
    user->current_sprite = sprites::damned[9];
  }
}

void DamnedGrab::playerHitCheck() {
  if (player->state == DEAD || player->parried_attack == true) {
    return;
  }

  if (CheckCollisionRecs(hurtbox, player->hitbox) == false) {
    return;
  }

  player->takeDamage(2, 2.0, 0.25);
  if (player->state == HIT_STUN) {
    player->enterHitStun(stun_time);
    grabbed_player = true;
  }
  else if (player->parried_attack) { 
    recovery_time = recovery_time * 2;
    player->incrementMorale(3);
  }

  user->state = RECOVER;
  sequence_timestamp = CURRENT_TIME; 
}

void DamnedGrab::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);

  if (finished_recovering == false) {
    return;
  }

  float minimum_ticks;
  float maximum_ticks;
  if (grabbed_player) {
    minimum_ticks = 15;
    maximum_ticks = 20;

    player->visible = true;
    player->setKnockback(1.0, user->direction);
  }
  else {
    minimum_ticks = 5;
    maximum_ticks = 10;
  }

  uniform_int_distribution<int> range(minimum_ticks, maximum_ticks);
  auto *enemy = static_cast<DamnedEnemy*>(user);
  enemy->cooldown_patience = range(RNG::generator);
}

void DamnedGrab::drawDebug() {
  if (user->state == ACT) {
    DrawRectangleLinesEx(hurtbox, 1, MAROON);
  }
}
