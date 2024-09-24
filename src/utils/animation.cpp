// utils/animation.cpp
#include <raylib.h>
#include <vector>
#include "base/actor.h"
#include "globals.h"
#include "utils_animation.h"

using std::vector;


void Animation::check(Actor *actor, const vector<Texture*> &sprite_list, 
                      vector<int> &frame_order)
{
  bool no_animation = actor->current_anim == NULL;

  if (no_animation || *actor->current_anim != frame_order) {
    actor->current_anim = &frame_order;
    actor->current_frame = frame_order.begin();

    int first_frame = *actor->current_frame;
    actor->current_sprite = sprite_list[first_frame];
    actor->frame_timestamp = CURRENT_TIME;
  }
}

void Animation::updateSprite(Actor *actor, 
                             const vector<Texture*> &sprite_list)
{
  int sprite_index = *actor->current_frame;
  actor->current_sprite = sprite_list[sprite_index];

  actor->frame_timestamp = CURRENT_TIME;
}

void Animation::play(Actor *actor, const vector<Texture*> &sprite_list, 
                     vector<int> &frame_order, const float frame_time, 
                     const bool looping) 
{
  const float time_elapsed = CURRENT_TIME - actor->frame_timestamp;
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
