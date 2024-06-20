// utils.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>
#include "base/generics.h"
#include "base/actor.h"
#include "char_player.h"

namespace CameraUtils {
  Camera2D setupCamera();

  /* For having the camera follow the player whenever they are off center.
   * This process only stops when the player is at the center of the 
   * screen again, or the camera has hit a boundary.*/
  void followPlayer(Camera2D &camera, PlayerCharacter &player, 
                    double &delta_time);
}


/* Functions that have everything to do with sprite animations. These
 * set of functions are compatible with the Actor class and anything other
 * class that derive from it.*/
namespace Animation {
  /* For checking if the animation should continue. Depending on a number
   * of conditions, this function may change the actor's current animation
   * and set their sprite to the first frame of said animation. This
   * usually happens when the Actor changes animations or it doesn't
   * have an animation set to begin with.*/
  void check(Actor *actor, std::vector<Texture*> &sprite_list,
             std::vector<int> &frame_order);

  /* Updates an actor's sprite by using an index gotten from an
   * actor's current_frame iterator. The actor will also save a
   * timestamp of the last time this function was called. The timestamp
   * will be used to check if enough time is passed to move on to the
   * next frame in the animation.*/
  void updateSprite(Actor *actor, std::vector<Texture*> &sprite_list);

  /* The root function for playing any animation for Actors and 
   * classes that derive from them. Setting looping to false will cause
   * actor to be left on the last frame of a animation unless they switch
   * to a new one.*/
  void play(Actor *actor, std::vector<Texture*> &sprite_list,
            std::vector<int> &frame_order, float frame_time, 
            bool looping = true);
}


/* Functions that are related to common AI behavior for enemies, bosses,
 * and any character that's not directly controlled by the player.*/
namespace AIBehavior {
  /* Patience is a common variable used by enemies and the like to 
   * determine whether or not they should act depending on it's value.
   * By default, this function is for automatically decrementing that 
   * value 20 times per second. Stopping at 0.*/
  void tickPatience(uint8_t &patience, float tick_timestamp, 
                    float tick_rate = 0.05);
}


namespace Enemies {
  /* Does exactly what is says on the tin. Automatically deletes enemies
   * within that are awaiting deletion, and creates a new list with the
   * deleted enemy excluded. Should be called once every frame.*/
  void deleteDeadEnemies(combatant_list &enemies);
}

