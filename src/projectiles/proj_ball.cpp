// projectiles/proj_ball.cpp
#include <raylib.h>
#include "base/action_command.h"
#include "globals.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include "char_player.h"
#include "cmd_ball_heavy.h"
#include "proj_ball.h"
#include <plog/Log.h>


BallProjectile::BallProjectile(Vector2 position, PlayerCharacter *player, 
                               combatant_list *enemies):
  DynamicActor(position, TYPE_PROJECTILE, {9, 9}, {13, 13}, {-4, -4.5}, 
               {-6, -6})
{
  this->player = player;
  this->enemies = enemies;
  this->direction = player->direction;

  damage = 2;
  stun_time = 0.2;

  velocity = 2.0;
  bounce_limit = 5;

  anim_spin = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  spin_frametime = 0.025;

  current_sprite = sprites::weapon_ball[0];
}

void BallProjectile::update() {
  Animation::play(this, sprites::weapon_ball, anim_spin, spin_frametime);
  movement();

  if (kickable == false) {
    enemyHitCheck();
  }
  else {
    playerKickCheck();
  }

  if (awaiting_deletion == false && can_bounce == true) {
    player->sub_weapon->disabled_timestamp = CURRENT_TIME;
  }
}

void BallProjectile::movement() {
  position.x += (velocity * DELTA_TIME) * direction;

  if (position.x < -CAMERA_BOUNDS || position.x > CAMERA_BOUNDS) {
    awaiting_deletion = true;
  }

  hitboxCorrection();
  texRectCorrection();
}

void BallProjectile::enemyHitCheck() {
  bool hit_enemy = false;

  for (auto &enemy : *enemies) {
    if (enemy->state == DEAD || enemy->state == HIT_STUN) {
      continue;
    }

    if (CheckCollisionRecs(hitbox, enemy->hitbox)) {
      enemy->takeDamage(damage, guard_pierce, stun_time);
      hit_enemy = true;
      break;
    }
  }

  if (hit_enemy == false) {
    return;
  }

  if (can_bounce == false) {
    SoundUtils::play("ball_heavy_chain", sound_pitch);
    return;
  }

  bounce_count++;

  if (bounce_count == bounce_limit) {
    can_bounce = false;

    SoundUtils::play("ball_heavy_chain", sound_pitch);
    PLOGI << "Bounce limit reached.";
  }
  else {
    kickable = true;
    direction *= -1;
  }
}

void BallProjectile::playerKickCheck() {
  if (player->state != ACT) {
    return;
  }

  auto command_type = player->current_command->type;
  bool using_heavy_tech = command_type == CMD_TECH_HEAVY;
  if (using_heavy_tech != true) {
    return;
  }

  auto *command = static_cast<BallHeavy*>(player->current_command.get());
  if (CheckCollisionRecs(hitbox, command->hurtbox)) {
    direction = player->direction;
    kickable = false;

    damage++;
    velocity += 0.25;

    SoundUtils::play("ball_heavy_chain", sound_pitch);
    sound_pitch += 0.1;

    SoundUtils::play("ball_heavy_kick");
  }
}

void BallProjectile::draw(Vector2 &camera_target) {
  Actor::draw(camera_target);
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  if (direction == -1) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE) {
    drawDebug();
  } 
}
