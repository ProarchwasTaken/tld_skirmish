// action_command/cmd_damned_grb.cpp
#include <cstddef>
#include <assert.h>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <random>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "utils_sound.h"
#include "enemy_damned.h"
#include "cmd_damned_grb.h"
#include <plog/Log.h>

using std::uniform_int_distribution;


DamnedGrab::DamnedGrab(DamnedEnemy *user):
  ActionCommand(user, "Damned Grab", CMD_NONE, 0.8, 0.1, 0.5) 
{
  this->player = user->player;

  stun_time = grab_time + 0.5;

  crashing_out = user->crashing_out;
  if (crashing_out) {
    performAdjustments();
  }
}

DamnedGrab::~DamnedGrab() {
  user->current_anim = NULL;
}

void DamnedGrab::performAdjustments() {
  momentum_direction = user->direction * -1;
  assert(momentum_direction != 0);

  charge_time = 0.4;

  if (player->state != NEUTRAL || player->moving == false) {
    max_momentum = 2.5;
  } 

  user->invulnerable = true;
  user->current_sprite = sprites::damned[7];
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

  if (crashing_out) {
    decelerate();
  }
  else {
    Animation::play(this->user, sprites::damned, anim_charge, 
                    charge_frametime);
  }

  if (finished_charge) {
    user->current_sprite = sprites::damned[8];
    user->invulnerable = false;
    setupHurtbox();
  }
}

void DamnedGrab::decelerate() {
  if (percentage == 0.0) {
    return;
  }

  const float momentum = Lerp(0, max_momentum, percentage);
  user->position.x += (momentum * momentum_direction) * DELTA_TIME;
  user->hitboxCorrection();
  user->texRectCorrection();

  percentage -= GetFrameTime() / charge_time;
  percentage = Clamp(percentage, 0, max_momentum);
}

void DamnedGrab::actSequence(float time_elapsed) {
  ActionCommand::actSequence(time_elapsed);

  playerHitCheck();

  if (grabbed_player) {
    player->visible = false;
    repositionPlayer();

    user->current_sprite = sprites::damned[10];
    recovery_time = grab_time;
    tick_timestamp = CURRENT_TIME;
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

  PLOGD << "Hurtbox has collided with player hitbox.";
  uint8_t morale_bonus = 3;

  if (crashing_out) {
    morale_bonus += 3;
  }

  bool grab_successful = grabCheck();
  if (grab_successful) {
    PLOGD << "Successfully grabbed the player.";
    player->enterHitStun(stun_time);
    player->setKnockback(0, user->direction, true);
    grabbed_player = true;
  }
  else if (player->parried_attack) { 
    PLOGD << "Player has parried the grab. Applying penalty.";
    recovery_time = recovery_time * 2;
    player->incrementMorale(morale_bonus);
  }

  user->state = RECOVER;
  sequence_timestamp = CURRENT_TIME; 
}

bool DamnedGrab::grabCheck() {
  PLOGD << "Performing grab check.";
  bool sufficent_stun = player->getStunTime() >= stun_time;
  bool already_grabbed = player->state == HIT_STUN && sufficent_stun;

  if (already_grabbed) {
    PLOGD << "Player is already grabbed."
    " Proceeding to inflict basic damage.";
    player->takeDamage(6, 0.0, 0.0);
    return false;
  }
  else {
    player->takeDamage(2, 2.0, 0.25);
  }

  if (player->state == HIT_STUN) {
    return true;
  }
  else {
    return false;
  }
}

void DamnedGrab::repositionPlayer() {
  player->position.x = user->position.x + (29 * user->direction);
  player->hitboxCorrection();
  player->texRectCorrection();
}

void DamnedGrab::recoverySequence(float time_elapsed) {
  ActionCommand::recoverySequence(time_elapsed);

  if (grabbed_player) {
    tickDamage();
    struggleCheck();
    overkillCheck();
  }

  if (finished_recovering) {
    resetCooldown();
  }
}

void DamnedGrab::tickDamage() {
  float time_elapsed = CURRENT_TIME - tick_timestamp;
  if (time_elapsed >= tick_time) {
    player->takeDamage(tick_damage, 0.0, 0.0);
    SoundUtils::play("dam_damage_tick");

    tick_timestamp = CURRENT_TIME;
  }
}

void DamnedGrab::struggleCheck() {
  if (player->health == 0) {
    return;
  } 

  float time_elapsed = CURRENT_TIME - input_timestamp;
  if (time_elapsed < input_delay) {
    return;
  }

  int input = GetKeyPressed() || GetGamepadButtonPressed();
  if (input != 0) {
    progress++;

    SoundUtils::play("dam_struggle");
    input_timestamp = CURRENT_TIME;
  }

  if (progress >= threshold) {
    PLOGD << "Player has successfully broken free of grab.";
    player->enterHitStun(0.25);
    resetCooldown();
    
    SoundUtils::play("dam_struggle_free");
    user->enterHitStun(2.0);
  }
}

void DamnedGrab::overkillCheck() {
  bool overkill = player->health == 0 && player->combo >= 5;
  if (overkill && player->state == HIT_STUN) {
    PLOGD << "Overkill condition reached.";
    finished_recovering = true;
    player->endStunSequence();
    user->state = NEUTRAL;
  }
}

void DamnedGrab::resetCooldown() {
  float minimum_ticks;
  float maximum_ticks;

  if (grabbed_player) {
    minimum_ticks = 15;
    maximum_ticks = 20;

    player->visible = true;
    player->setKnockback(1.0, user->direction);
    SoundUtils::play("dam_release");
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
