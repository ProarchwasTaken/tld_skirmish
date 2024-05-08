// actor.h
#pragma once
#include <raylib.h>


class Actor {
public:
  Actor(Vector2 position, Vector2 hitbox_scale = {32, 64}, 
        Vector2 tex_scale = {64, 64});

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

  Vector2 position;
protected:
  Rectangle hitbox;
  Vector2 hitbox_position;
  Vector2 hitbox_scale;

  Rectangle tex_rect;
  Vector2 tex_position;
  Vector2 tex_scale;
};
