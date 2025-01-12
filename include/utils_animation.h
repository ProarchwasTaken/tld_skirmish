// utils_animation.h
#pragma once
#include <raylib.h>
#include <vector>
#include <triblib/base/actor.h>


/* Functions that have everything to do with sprite animations. This
 * set of functions are compatible with the Actor class, and all other
 * classes that derive from it.*/
namespace Animation {
  /* For checking if the animation should continue. Depending on a number
   * of conditions, this function may change the actor's current animation
   * and set their sprite to the first frame of said animation. This
   * usually happens when the Actor changes animations or it doesn't
   * have an animation set to begin with.*/
  void check(Actor *actor, const std::vector<Texture*> &sprite_list,
             std::vector<int> &frame_order);

  /* Updates an actor's sprite by using an index gotten from an
   * actor's current_frame iterator. The actor will also save a
   * timestamp of the last time this function was called. The timestamp
   * will be used to check if enough time is passed to move on to the
   * next frame in the animation.*/
  void updateSprite(Actor *actor, 
                    const std::vector<Texture*> &sprite_list);

  /* The root function for playing any animation for Actors and 
   * classes that derive from them. Setting looping to false will cause
   * actor to be left on the last frame of a animation unless they switch
   * to a new one.*/
  void play(Actor *actor, const std::vector<Texture*> &sprite_list,
            std::vector<int> &frame_order, const float frame_time, 
            const bool looping = true);
}

