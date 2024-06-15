// utils.cpp
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "defaults.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "char_player.h"
#include "utils.h"
#include <plog/Log.h>

using std::vector, std::shared_ptr, std::find;


Camera2D CameraUtils::setupCamera() {
  Camera2D camera;
  camera.target = {0, 120};
  camera.offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  camera.zoom = 1;
  camera.rotation = 0;

  return camera;
}

void CameraUtils::followPlayer(Camera2D &camera, PlayerCharacter &player,
                            double &delta_time) 
{
  bool player_offCenter = camera.target.x != player.position.x;
  if (player_offCenter == false) {
    return;
  }

  float x_difference = player.position.x - camera.target.x;
  float next_x = x_difference / 8;
  int direction;

  if (x_difference > 0) {
    direction = 1;
  }
  else if (x_difference < 0) {
    direction = -1;
  }

  float boundary = CAMERA_BOUNDS * direction;
  float half_width = CANVAS_WIDTH / 2.0;
  float screen_side = camera.target.x + (half_width * direction);

  bool already_at_boundary = screen_side == boundary; 
  if (already_at_boundary) {
    return;
  }

  float offset = screen_side + next_x;

  bool hit_left_bounds = offset <= -CAMERA_BOUNDS;
  bool hit_right_bounds = offset >= CAMERA_BOUNDS;
  if (hit_left_bounds) {
    camera.target.x = -CAMERA_BOUNDS + half_width; 
  }
  else if (hit_right_bounds) {
    camera.target.x = CAMERA_BOUNDS - half_width;
  }
  else {
    camera.target.x += next_x * delta_time;
    if (FloatEquals(camera.target.x, player.position.x)) {
      camera.target.x = player.position.x;
    }
  }
}


void Animation::check(Actor *actor, vector<Texture*> &sprite_list, 
                      vector<int> &frame_order)
{
  bool no_animation = actor->current_anim == NULL;

  if (no_animation || *actor->current_anim != frame_order) {
    actor->current_anim = &frame_order;
    actor->current_frame = frame_order.begin();

    int first_frame = *actor->current_frame;
    actor->current_sprite = sprite_list[first_frame];
    actor->frame_timestamp = GetTime();
  }
}

void Animation::updateSprite(Actor *actor, vector<Texture*> &sprite_list){
  int sprite_index = *actor->current_frame;
  actor->current_sprite = sprite_list[sprite_index];

  actor->frame_timestamp = GetTime();
}

void Animation::play(Actor *actor, vector<Texture*> &sprite_list, 
                     vector<int> &frame_order, float frame_time, 
                     bool looping) 
{
  float time_elapsed = GetTime() - actor->frame_timestamp;
  if (time_elapsed < frame_time) {
    return;
  }

  Animation::check(actor, sprite_list, frame_order);

  bool end_of_animation = actor->current_frame == frame_order.end();
  if (end_of_animation == false) {
    Animation::updateSprite(actor, sprite_list);
    actor->current_frame++;

    end_of_animation = actor->current_frame == frame_order.end();
  }
 
  if (looping == false) {
    return;
  }

  if (end_of_animation) {
    actor->current_frame = frame_order.begin();
  }
}

void deleteDeadEnemies(combatant_list &enemies) {
  int enemy_count = enemies.size();
  if (enemy_count == 0) {
    return;
  }

  vector<int> deleted; 

  for (int index = 0; index < enemy_count; index++) {
    shared_ptr<Combatant> enemy = enemies[index];

    if (enemy->awaiting_deletion) {
      PLOGD << "Deleting enemy: " << enemy->name << " from memory.";
      enemy.reset();

      PLOGI << "Appending enemy index: " << index << 
        " to 'deleted' list.";
      deleted.push_back(index);
    }
  }

  if (deleted.size() == 0) {
    return;
  }

  PLOGI << "Transfering all enemies to new list with the deleted enemies" 
  " excluded.";
  combatant_list new_enemy_list;
  vector<int>::iterator search_result;

  for (int index = 0; index < enemy_count; index++) {
    search_result = find(deleted.begin(), deleted.end(), index);
    bool is_excluded = search_result != deleted.end();

    if (is_excluded == false) {
      new_enemy_list.push_back(enemies[index]);
    }
  }

  // I'm not exactly sure whether replacing the enemies list with the new
  // one is gonna cause a memory leak or not.
  enemies.clear();
  enemies = new_enemy_list;
}
