// actor.h
#pragma once
#include <cstddef>
#include <raylib.h>
#include <vector>


/* The Actor Base class is the root of pretty much every game object that
 * serves a purpose that's more than just being decoration. Some may
 * refer to this as an abstract class, only meant to be expanded upon by
 * other base classes.*/
class Actor {
public:
  Actor(Vector2 position, Vector2 hitbox_scale, Vector2 tex_scale, 
        Vector2 hitbox_offset, Vector2 tex_offset);

  /* The hitbox is used for checking for collision with the level's 
   * boundaries, etc. Whenever an actors position changes, this method 
   * must be called to maintain accuracy.*/
  void hitboxCorrection();
  
  /* The texture rect and position is used to determine where on the
   * screen should the actor be drawn. With the former being used to
   * check if the actor is on-screen. Like it's corresponding method,
   * this must be called whenever the actor's base position changes in
   * any way.*/
  void texRectCorrection();

  virtual void draw() {};
  virtual void drawDebug();

  Texture *current_sprite;
  std::vector<int> *current_anim = NULL;
  float frame_timestamp = 0;

  std::vector<int>::iterator current_frame;

  Vector2 position;
  Rectangle hitbox;
private:
  Vector2 tex_offset;
  Vector2 hitbox_offset;
protected:
  Rectangle tex_rect;
  Vector2 tex_position;
  Vector2 tex_scale;

  Vector2 hitbox_position;
  Vector2 hitbox_scale;
};
