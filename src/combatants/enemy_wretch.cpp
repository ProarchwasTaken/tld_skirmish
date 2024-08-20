// combatants/enemy_wretch.cpp
#include <cstdlib>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <cstdint>
#include <random>
#include "globals.h"
#include "utils.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_wretch_atk.h"
#include "char_player.h"
#include "enemy_wretch.h"

using std::uniform_int_distribution, std::make_unique, std::unique_ptr;

uniform_int_distribution<int> wrh_patience_range(
  WRH_MIN_PATIENCE, WRH_MAX_PATIENCE
);


WretchEnemy::WretchEnemy(PlayerCharacter &player, Vector2 position):
  Combatant("Wretch", TYPE_ENEMY, WRH_HP, WRH_STABILITY, position,
            WRH_HIT_SCALE, WRH_TEX_SCALE, WRH_HIT_OFFSET, WRH_TEX_OFFSET)
{
  this->player = &player;
  current_sprite = sprites::wretch[0];

  movement_speed = 0.4;
  preferred_dist = 64;

  anim_idle = {0, 1, 2, 1};
  idle_frametime = 0.3;

  anim_retreat = {3, 4, 5, 4};
  retreat_frametime = 0.250;

  anim_death = {7, 8};
  death_frametime = 0.5;
}

void WretchEnemy::update() {
  switch (state) {
    case NEUTRAL: {
      neutralBehavior();
      break;
    }
    case HIT_STUN: {
      current_sprite = sprites::wretch[6];

      applyKnockback(512);
      stunSequence();
      break;
    }
    case DEAD: {
      deathSequence(sprites::wretch, anim_death, death_frametime);
      break;
    }
    default: {
      commandSequence();
    } 
  }
}

void WretchEnemy::neutralBehavior() {
  int16_t x_offset = player->position.x - position.x;
  player_dist = std::abs(x_offset);
  direction = Clamp(x_offset, -1, 1);

  if (retreat_patience == 0) {
    Animation::play(this, sprites::wretch, anim_idle, idle_frametime);
    pursue();
  }
  else {
    Animation::play(this, sprites::wretch, anim_retreat, 
                    retreat_frametime);

    position.x -= ((movement_speed * 2) * direction) * DELTA_TIME;
    AIBehavior::tickPatience(retreat_patience, tick_timestamp);
  }

  hitboxCorrection();
  texRectCorrection();
}

void WretchEnemy::pursue() {
  bool within_range = player_dist <= preferred_dist;
  bool should_attack = within_range && player->state != DEAD;

  if (should_attack) {
    retreat_patience = wrh_patience_range(RNG::generator);

    unique_ptr<ActionCommand> command;
    command = make_unique<WretchAttack>(this);
    useCommand(command);
    return;
  }

  position.x += (movement_speed * direction) * DELTA_TIME;
}

void WretchEnemy::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  if (direction == LEFT) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  }
}

void WretchEnemy::drawDebug() {
  Actor::drawDebug();

  Enemies::drawPatience(this, retreat_patience, BLUE, 0);

  if (isUsingCommand()) {
    current_command->drawDebug();
  }
}

