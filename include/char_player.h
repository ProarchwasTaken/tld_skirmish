// char_player.h
#pragma once
#include <cstdint>
#include <raylib.h>

#define RIGHT 1
#define LEFT -1


/* The playable character, the controllable avatar for the user. The most
 * important game object as it wouldn't be a game without one.*/
class PlayerCharacter {
public:
  PlayerCharacter();

  void update(double &delta_time);
  void draw();

  void hitboxCorrection();
  void texRectCorrection();

  void movement(double &delta_time);
  bool isMoving();

  void inputPressed();
  void inputReleased();

  bool moving;
  float movement_speed;
  int8_t direction;
private:
  Vector2 position;

  Rectangle hitbox;
  Vector2 hitbox_position;
  Vector2 hitbox_scale;

  Rectangle tex_rect;
  Vector2 tex_position;
  Vector2 tex_scale;

  bool moving_right = false;
  bool moving_left = false;
};
