// combatants/enemy_damned.cpp
#include <cassert>
#include <cmath>
#include <cstdint>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils_ai_behavior.h"
#include "utils_camera.h"
#include "utils_animation.h"
#include "utils_enemies.h"
#include "utils_sound.h"
#include "char_player.h"
#include "cmd_damned_grb.h"
#include "enemy_damned.h"

using std::unique_ptr, std::make_unique;


DamnedEnemy::DamnedEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Damned", TYPE_ENEMY, DAM_HP, DAM_STABILITY, position,
            DAM_HIT_SCALE, DAM_TEX_SCALE, DAM_HIT_OFFSET, DAM_TEX_OFFSET)
{
  this->player = &player;
  current_sprite = sprites::damned[1];

  anim_walk = {0, 1, 2, 1};
  walk_frametime = 0.5;

  anim_run = {11, 12, 13, 12};
  run_frametime = 0.01;

  anim_death = {4, 5};
  death_frametime = 0.5;

  step_distance = 10;
  preferred_dist = 35;
  crashout_dist = 75;
}

void DamnedEnemy::update() {
  switch (state) {
    case NEUTRAL: {
      neutralBehavior();
      break;
    } 
    case HIT_STUN: {
      current_sprite = sprites::damned[3];

      applyKnockback(512);
      stunSequence();
      break;
    }
    case DEAD: {
      deathSequence(sprites::damned, anim_death, death_frametime);
      break;
    }
    default: {
      commandSequence();
    }
  }
}

void DamnedEnemy::neutralBehavior() {
  if (shouldProceed() == false) {
    current_sprite = sprites::damned[6];
    return;
  }

  bool moving_away = player->direction == direction && player->moving;
  bool states_apply = crashing_out == false && player->state == NEUTRAL;
  if (moving_away && states_apply) {
    AIBehavior::tickPatience(crashout_patience, tick_timestamp);
  }

  if (crashing_out == false && shouldCrashout()) {
    crashout();
  }

  if (crashing_out == false) {
    normalProcedure();
  }
  else {
    crashoutProcedure();
  }
}

bool DamnedEnemy::shouldProceed() {
  if (player->state == DEAD) {
    return false;
  } 
  else if (cooldown_patience != 0) {
    AIBehavior::tickPatience(cooldown_patience, tick_timestamp);
    return false;
  }
  else {
    return true;
  }
}

void DamnedEnemy::updateDirection(const float x_offset) {
  int8_t new_direction = Clamp(x_offset, -1, 1);
  if (new_direction == 0) {
    return;
  }
  else {
    direction = new_direction;
  }
}

void DamnedEnemy::normalProcedure() {
  float time_elapsed = CURRENT_TIME - frame_timestamp;
  if (time_elapsed < walk_frametime) {
    return;
  }

  x_offset = player->position.x - position.x;
  player_dist = std::abs(x_offset);
  updateDirection(x_offset);

  if (player_dist > preferred_dist) {
    stepForward();
    Animation::play(this, sprites::damned, anim_walk, walk_frametime);
  }
  else {
    attemptGrab();
  }
}

void DamnedEnemy::stepForward() {
  position.x += step_distance * direction;

  float volume = Clamp((x_offset / 512.0) + 1.1, 0, 1);
  float pan = Clamp((x_offset / 512.0) + 0.5, 0, 1);
  SoundUtils::playPro("dam_footstep", volume, 1.0, pan);

  hitboxCorrection();
  texRectCorrection();
}

void DamnedEnemy::attemptGrab() {
  unique_ptr<ActionCommand> command;

  if (crashing_out == false) {
    SoundUtils::play("dam_alert");
  }
  else {
    SoundUtils::play("dam_alert_alt");
  }

  command = make_unique<DamnedGrab>(this);
  useCommand(command);
}

bool DamnedEnemy::shouldCrashout() {
  if (crashout_patience != 0) {
    return false;
  }

  int16_t x_offset = player->position.x - position.x;
  float distance = std::abs(x_offset);

  if (distance > crashout_dist) {
    return true;
  }
  else {
    return false;
  }
}

void DamnedEnemy::crashout() {
  crashing_out = true;
  current_sprite = sprites::damned[3];

  float volume = Clamp((x_offset / 512.0) + 1.1, 0, 1);
  float pan = Clamp((x_offset / 512.0) + 0.5, 0, 1);

  SoundUtils::playPro("dam_screech", volume, 1.0, pan);
  crashout_timestamp = CURRENT_TIME;
}

void DamnedEnemy::crashoutProcedure() {
  float since_crashout = CURRENT_TIME - crashout_timestamp;
  if (since_crashout < crashout_time) {
    return;
  }

  float time_elapsed = CURRENT_TIME - frame_timestamp;
  if (time_elapsed < run_frametime) {
    return;
  }

  x_offset = player->position.x - position.x;
  player_dist = std::abs(x_offset);

  int8_t old_direction = direction;
  updateDirection(x_offset);

  bool passed_player = old_direction != direction;
  if (passed_player) {
    attemptGrab();

    crashing_out = false;
    crashout_patience = 30;
  }
  else {
    stepForward();
    Animation::play(this, sprites::damned, anim_run, run_frametime);
  }
}

void DamnedEnemy::takeDamage(uint16_t dmg_magnitude, float guard_pierce,
                             float stun_time, float kb_velocity, 
                             int8_t kb_direction) 
{
  Combatant::takeDamage(dmg_magnitude, guard_pierce, stun_time, 
                        kb_velocity, kb_direction);

  if (crashing_out && state == HIT_STUN) {
    player->incrementMorale(1);
  }
}

void DamnedEnemy::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  if (direction == LEFT) {
    source.width *= -1;
  }

  Rectangle dest = tex_rect;
  if (state == RECOVER) {
    shakeEffect(dest);
  }

  DrawTexturePro(*current_sprite, source, dest, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  }
}

void DamnedEnemy::shakeEffect(Rectangle &dest) {
  assert(state == RECOVER);

  if (player->health == 0) {
    return;
  }

  auto *command = static_cast<DamnedGrab*>(current_command.get());
  if (command->grabbed_player) {
    float time_elapsed = CURRENT_TIME - command->sequence_timestamp;
    float sinY = std::sin(time_elapsed * 20) * 0.55;
    dest.x += sinY;
  }
}

void DamnedEnemy::drawDebug() {
  Actor::drawDebug();

  Enemies::drawPatience(this, cooldown_patience, BLUE, 0);
  Enemies::drawPatience(this, crashout_patience, ORANGE, 10);

  if (isUsingCommand()) {
    current_command->drawDebug();
  }
}
