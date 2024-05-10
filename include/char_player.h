// char_player.h
#pragma once
#include <cstdint>
#include <vector>
#include <raylib.h>
#include "base/combatant.h"

#define RIGHT 1
#define LEFT -1

#define PLAYER_BOUNDS 384

#define BTN_LIGHT_ATK 0


/* The playable character, the controllable avatar for the user. The most
 * important game object as it wouldn't be a game without one.
 *
 * Like almost everything else, the player character derives froms the
 * Combatant base class, so it would have the usual stuff you would
 * expect like: health, variable for what state they are in, variables for
 * for other elements of the game to use to identify what they are, and
 * a method for assigning action commands.
 *
 * One notable difference is how the player character knows when to 
 * execute commands is through the usage of an input buffer. The input
 * buffer keeps track of any notable button inputs from the user within 
 * a certain amount of time after the first input has been detected.
 * When it's time to clear the input buffer, and the player is in a
 * neutral state, the input buffer will be read, and the player will
 * decide what action command to use from there.*/
class PlayerCharacter : public Combatant {
public:
  PlayerCharacter();
  ~PlayerCharacter();

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

  /* For checking if the input_buffer has detected the first input. If so,
   * the timer will start, beginning the whole sequence.*/
  void bufferTimerCheck();

  /* Possibly the only way action commands would be assigned to the 
   * player. Reads the input buffer and decides what action command to
   * assign to the player based on that. This process is only done when
   * the timer has started, and in the frame of which the buffer is about
   * to be cleared.*/
  void interpretBuffer();

  /* For clearing the buffer onces the timer runs out. That's it.
   * possibly the simplest of the 3 methods.*/
  void clearBufferCheck();

  bool moving;
  float movement_speed;
  int8_t direction;
private:
  std::vector<uint8_t> input_buffer;

  bool buf_empty = false;
  bool buf_timer_started = false;

  float buf_clear_time;
  float buf_input_timestamp;

  bool moving_right = false;
  bool moving_left = false;
};
