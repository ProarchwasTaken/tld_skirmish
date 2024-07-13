// char_player.h
#pragma once
#include <cstdint>
#include <vector>
#include <raylib.h>
#include "base/generics.h"
#include "base/combatant.h"

#define PLR_BOUNDS 384

#define PLR_HP 100
#define PLR_STABILITY 0.5
#define PLR_START_POS (Vector2){0, 152}
#define PLR_HITBOX_SCALE (Vector2){16, 56}
#define PLR_HITBOX_OFFSET (Vector2){-8, -58}

#define BTN_LIGHT_ATK 0
#define BTN_HEAVY_ATK 1
#define BTN_GUARD 2


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
  PlayerCharacter(combatant_list &enemies);
  ~PlayerCharacter();

  /* Is called once every frame. Typically all of the player logic goes
   * in here.*/
  void update(double &delta_time) override;
  void draw() override;
  void drawDebug() override;

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

  /* The root method for interpreting the input buffer and decided what 
   * command should assigned to the player basing on the number of
   * circumstances. It's full functionality is triggered once the buffer
   * is about to clear, it may use normal or special logic depending
   * on what state the player is in.*/
  void interpretBuffer();

  /* This method is only called when the player is in the neutral state.
   * It checks the first to second inputs in the buffer, and decides what
   * command to assign to the player based on that.*/
  void normalInterpretLogic();

  /* Special interpret logic is quite special as it's only used when the
   * player is currently performing an action command. Different 
   * logic may be also used depending on what action command is being
   * used as well.*/
  void specialInterpretLogic();

  /* Only called during special interpret logic, and the player is using
   * the LightAttack. For checking if the player should cancel the
   * recovery phase of the attack and immediately perform a heavy attack.
   * That's only if certain conditions are met.*/
  void lightAttackHandling();

  /* For clearing the buffer onces the timer runs out. That's it.
   * possibly the simplest of the 3 methods.*/
  void clearBufferCheck();

  combatant_list *enemies;

  bool moving;
  float movement_speed;
private:
  std::vector<uint8_t> input_buffer;

  bool buf_empty = false;
  bool buf_timer_started = false;

  float buf_clear_time;
  float buf_input_timestamp;

  std::vector<int> anim_walk;
  float walk_frametime;

  bool moving_right = false;
  bool moving_left = false;
};
