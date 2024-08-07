// actor.h
#pragma once
#include <cstddef>
#include <raylib.h>
#include <vector>


/* The Actor Base class is the root of pretty much every game 
 * object/character. It's only meant to function as the core for other 
 * classes. Instances of this base class and all classes that derive from 
 * it are capable of using sprites and animations.*/
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

  /* Doesn't actually draw anything, but instead contains assertion 
   * statements that stops someone the inevitably forgetting something 
   * like assigning a default sprite. Resulting in a seqfault that could 
   * take hours to track down. (Example has no relation to any past 
   * events.)*/
  virtual void draw(Vector2 &camera_target);

  /* Basically draws the actor's hitbox, and texture rect on the screen.
   * Must only be used for debug purposes only.*/
  virtual void drawDebug();

  Texture *current_sprite = NULL;
  std::vector<int> *current_anim = NULL;
  float frame_timestamp = 0;

  std::vector<int>::iterator current_frame;

  Vector2 position;
  Rectangle hitbox;
  Rectangle tex_rect;

  Vector2 tex_offset;
  Vector2 hitbox_offset;
protected:
  Vector2 tex_position;
  Vector2 tex_scale;

  Vector2 hitbox_position;
  Vector2 hitbox_scale;
};
