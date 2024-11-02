// action_command/cmd_gun_light.cpp
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include "globals.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"
#include "utils_animation.h"
#include "cmd_gun_light.h"
#include <plog/Log.h>

using std::vector, std::min_element;


GunLight::GunLight(PlayerCharacter *player): 
  ActionCommand(player, "Gun Light", CMD_TECH_LIGHT, 0.3, 0.2, 0.25) 
{
  this->player = player;
  this->enemies = player->enemies;

  player->current_sprite = sprites::player[28];
}

GunLight::~GunLight() {
  // I have to reiterate, very important that this is set to NULL upon
  // deletion. I can not stress this enough.
  player->current_anim = NULL;
}

Rectangle GunLight::setupHurtbox() { 
  float width = max_range;
  float half_width = width / 2;

  float height = 4;

  float x_offset = half_width * user->direction;

  float x = (user->position.x - half_width) + x_offset;
  float y = user->position.y - 52;

  Rectangle hurtbox = {x, y, width, height};
  return hurtbox;
}

void GunLight::chargeSequence(float time_elapsed) {
  ActionCommand::chargeSequence(time_elapsed);

  Animation::play(player, sprites::player, anim_charge, charge_frametime);

  if (finished_charge) {
    player->current_sprite = sprites::player[29];
    enemyHitCheck();
  }
}

void GunLight::enemyHitCheck() {
  vector<Combatant*> detected_enemies;
  enemyDetection(&detected_enemies);

  int count = detected_enemies.size(); 
  PLOGD << "Enemies Detected: " << count;
  if (count == 0) {
    return;
  }
  else if (count == 1) {
    probed_enemy = detected_enemies.front();
    hit_enemy = true;
  }
  else {
    PLOGI << "Enemies detected is more than 1.";
    probeClosestEnemy(detected_enemies);
  }

  assert(probed_enemy != NULL);
  PLOGI << "Probed Enemy: " << probed_enemy->name;
}

void GunLight::enemyDetection(vector<Combatant*> *detected_enemies) {
  assert(detected_enemies->size() == 0 && "vector was not empty!");
  PLOGI << "Gathering a list of enemies that are within range.";

  Rectangle hurtbox = setupHurtbox();
  for (auto enemy : *enemies) {
    if (enemy->state == DEAD) {
      continue;
    }

    if (CheckCollisionRecs(hurtbox, enemy->hitbox)) {
      detected_enemies->push_back(enemy.get());
    }
  }
} 

void GunLight::probeClosestEnemy(vector<Combatant*> &detected_enemies) {
  PLOGI << "Attempting to probe the enemy closest to the player.";
  vector<float> player_dists;

  for (Combatant *enemy : detected_enemies) {
    float x_difference = enemy->position.x - player->position.x;
    player_dists.push_back(std::abs(x_difference));
  }
  PLOGD << "Gathered player distance between each enemy.";

  vector<float>::iterator closest_dist;
  closest_dist = min_element(player_dists.begin(), player_dists.end());
  PLOGD << "Closest distance: " << *closest_dist;

  int index = std::distance(player_dists.begin(), closest_dist);

  PLOGD << "Closest enemy index: " << index;
  assert(index <= detected_enemies.size() - 1);

  probed_enemy = detected_enemies[index];
  hit_enemy = true;
}

void GunLight::actSequence(float time_elapsed) {
  if (hit_enemy && techInputHeldDown()) {
    slowMovement();
    tickDamage();
    rangeCheck();
    return;
  }

  ActionCommand::actSequence(time_elapsed);

  if (finished_action) {
    player->current_sprite = sprites::player[32];
  }
}

bool GunLight::techInputHeldDown() {
  bool key_a = IsKeyDown(KEY_A);
  bool btn_face_left = false;

  if (IsGamepadAvailable(0)) {
    btn_face_left = IsGamepadButtonDown(0, 
                                        GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
  }

  return key_a || btn_face_left;
}

void GunLight::slowMovement() {
  int8_t move_direction = 0;
  if (player->moving_right) {
    move_direction = RIGHT;
  }
  else if (player->moving_left) {
    move_direction = LEFT;
  }

  player->moving = player->isMoving(anim_slow, slow_frametime, true,
                                    sprites::player[29]);
  if (move_direction != 0) {
    player->movement(player->movement_speed * 0.5, false, 
                     &move_direction);
  }
}

void GunLight::detachProbes() {
  player->current_sprite = sprites::player[32];
  player->state = RECOVER;
  sequence_timestamp = CURRENT_TIME;
}

void GunLight::tickDamage() {
  float time_elapsed = CURRENT_TIME - tick_timestamp;

  if (time_elapsed < tick_time) {
    return;
  }

  assert(probed_enemy != NULL);
  probed_enemy->takeDamage(tick_damage, 0.25, 1.0);

  if (player->morale < tick_cost) {
    detachProbes();
  }
  else {
    player->morale -= tick_cost;
  }

  tick_timestamp = CURRENT_TIME;
}

void GunLight::rangeCheck() {
  float x_offset = probed_enemy->position.x - player->position.x;
  float distance = std::abs(x_offset);

  if (distance > max_range) {
    detachProbes();
  }
}
