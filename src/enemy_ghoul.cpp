// enemy_ghoul.cpp
#include <cstdlib>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include "globals.h"
#include "utils.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_ghoul_atk.h"
#include "char_player.h"
#include "enemy_ghoul.h"

using std::make_unique, std::unique_ptr;


GhoulEnemy::GhoulEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Ghoul", TYPE_ENEMY, GOL_HP, position, GOL_HITBOX_SCALE,
            {64, 64}, GOL_HITBOX_OFFSET) 
{
  this->player = &player;
  current_sprite = sprites::ghoul[0];

  anim_walk = {0, 1, 2, 1};
  walk_frametime = 0.125;

  anim_death = {7, 8};
  death_frametime = 0.5;

  death_time = 1.0;

  preferred_dist = 28;
  movement_speed = 1.25;
}

void GhoulEnemy::update(double &delta_time) {
  switch (state) {
    case NEUTRAL: {
      neutralBehavior(delta_time);
      break;
    }
    case HIT_STUN: {
      current_sprite = sprites::ghoul[3];
      stunSequence();
      break;
    }
    case DEAD: {
      deathSequence();
      break;
    }
    default: {
      commandSequence();
    }
  }
}

void GhoulEnemy::neutralBehavior(double &delta_time) {
  int16_t x_offset = player->position.x - position.x;
  player_dist = std::abs(x_offset);
  direction = Clamp(x_offset, -1, 1);

  if (player_dist > preferred_dist) {
    movement(delta_time);
    Animation::play(this, sprites::ghoul, anim_walk, walk_frametime);
  }
  else { 
    unique_ptr<ActionCommand> command;
    command = make_unique<GhoulAttack>(this);
    useCommand(command);
  }
}

void GhoulEnemy::movement(double &delta_time) {
  position.x += (movement_speed * direction) * delta_time;
  hitboxCorrection();
  texRectCorrection();
}

void GhoulEnemy::deathSequence() {
  Animation::play(this, sprites::ghoul, anim_death, death_frametime,
                  false);

  float time_elapsed = GetTime() - death_timestamp;
  bool end_of_animation = current_frame == current_anim->end();

  if (end_of_animation && time_elapsed >= death_time) {
    awaiting_deletion = true;
  }
}

void GhoulEnemy::draw() {
  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  Rectangle dest = {tex_position.x, tex_position.y, 
    tex_scale.x, tex_scale.y};
  if (direction == LEFT) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, dest, {0, 0}, 0, WHITE);

  if (DEBUG_MODE == true) {
    drawDebug();
  }
}

void GhoulEnemy::drawDebug() {
  Actor::drawDebug();

  bool using_command;
  switch (state) {
    case NEUTRAL:
    case HIT_STUN:
    case DEAD: {
      using_command = false;
      break;
    }
    default: {
      using_command = true;
    }
  }

  if (using_command) {
    current_command->drawDebug();
  }
}
