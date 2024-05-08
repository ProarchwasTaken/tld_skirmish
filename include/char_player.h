// char_player.h
#pragma once
#include <cstdint>
#include <raylib.h>
#include "base/actor.h"

#define RIGHT 1
#define LEFT -1

#define PLAYER_BOUNDS 384

/* The playable character, the controllable avatar for the user. The most
 * important game object as it wouldn't be a game without one.*/
class PlayerCharacter : public Actor{
public:
  PlayerCharacter();

  /* Is called once every frame. Typically all of the player logic goes
   * in here.*/
  void update(double &delta_time);
  void draw();
  void debugDraw();

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
  bool moving_right = false;
  bool moving_left = false;
};
