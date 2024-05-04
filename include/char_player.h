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

  /* Is called once every frame. Typically all of the player logic goes
   * in here.*/
  void update(double &delta_time);
  void draw();

  /* The hitbox is used for checking for collision with the level's 
   * boundaries, or enemy attacks. Whenever the player's position changes,
   * this method must be called to maintain accuracy.*/
  void hitboxCorrection();

  /* The texture rect and position is used to determine where on the
   * screen should the player be drawn. With the former being used to
   * check if the player is on-screen. Like it's corresponding method,
   * this must be called whenever the player's base position changes in
   * any way.*/
  void texRectCorrection();

  /* For moving the player left or right. The direction the player moves
   * in is determined by two booleans which can be altered by user input.
   * Prevents the player from moving out of bounds when needed.*/
  void movement(double &delta_time);

  /* Returns true if the player character should be moving. The outcome is
   * determined by user input.*/
  bool isMoving();

  /* These use methods are for checking for input from the user, whether
   * the button was pressed or released, and doing stuff based on that.
   * If a gamepad is detected, the method will check for input from that
   * as well. If the player were to be in some special state, input will
   * be automatically be ignored.*/
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
