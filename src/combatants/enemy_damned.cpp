// combatants/enemy_damned.cpp
#include <cassert>
#include <cmath>
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

  anim_death = {4, 5};
  death_frametime = 0.5;

  step_distance = 10;
  preferred_dist = 35;
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
  if (player->state == DEAD) {
    current_sprite = sprites::damned[6];
    return;
  } 
  else if (cooldown_patience != 0) {
    AIBehavior::tickPatience(cooldown_patience, tick_timestamp);
    current_sprite = sprites::damned[6];
    return;
  }

  float time_elapsed = CURRENT_TIME - frame_timestamp;
  if (time_elapsed < walk_frametime) {
    return;
  }

  int16_t x_offset = player->position.x - position.x;
  direction = Clamp(x_offset, -1, 1);
  player_dist = std::abs(x_offset);

  if (player_dist > preferred_dist) {
    stepForward();

    float volume = Clamp((x_offset / 512.0) + 1.1, 0, 1);
    float pan = Clamp((x_offset / 512.0) + 0.5, 0, 1);
    SoundUtils::playPro("dam_footstep", volume, 1.0, pan);

    Animation::play(this, sprites::damned, anim_walk, walk_frametime);
  }
  else {
    unique_ptr<ActionCommand> command;

    SoundUtils::play("dam_alert");
    command = make_unique<DamnedGrab>(this);
    useCommand(command);
  }
}

void DamnedEnemy::stepForward() {
  position.x += step_distance * direction;
  hitboxCorrection();
  texRectCorrection();
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

  if (isUsingCommand()) {
    current_command->drawDebug();
  }
}
