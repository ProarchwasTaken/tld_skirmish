// char_player.h
#pragma once
#include <cstdint>
#include <vector>
#include <raylib.h>
#include <memory>
#include "base/generics.h"
#include "base/combatant.h"
#include "base/sub-weapon.h"

#define PLR_BOUNDS 384

#define PLR_HP 30
#define PLR_HP_CRITICAL 0.40
#define PLR_STABILITY 0.5
#define PLR_START_POS (Vector2){0, 152}
#define PLR_HITBOX_SCALE (Vector2){16, 56}
#define PLR_HITBOX_OFFSET (Vector2){-8, -58}

#define BTN_LIGHT_ATK 0
#define BTN_HEAVY_ATK 1
#define BTN_LIGHT_TECH 2
#define BTN_HEAVY_TECH 3
#define BTN_GUARD 4


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
  PlayerCharacter(combatant_list &enemies, uint8_t &phase);
  ~PlayerCharacter();

  /* Is called once every frame. Typically all of the player logic goes
   * in here.*/
  void update() override;

  void draw(Vector2 &camera_target) override;
  void drawDebug() override;

  /* For moving the player left or right. The direction the player moves
   * in is determined by two booleans which can be altered by user input.
   * Prevents the player from moving out of bounds when needed.*/
  void movement();

  /* For regenerating the player's health during the rest phase. Typically
   * increments the player's health by 1 at a set rate. Only stopping when
   * the player reaches max health.*/
  void regeneration();

  /* Makes sure that the value that the player's morale is incremented to
   * doesn't go past the maximum. The player's morale will increase
   * through certain actions, and this method is typically used for the
   * most part rather than just incrementing the value directly.*/
  void incrementMorale(uint8_t value);

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

  /* For clearing the buffer onces the timer runs out. That's it.
   * possibly the simplest of the 3 methods.*/
  void clearBufferCheck();

  /* This method is only called when the player is in the neutral state.
   * It checks the first input in the buffer, and decides what
   * command to assign to the player based on that.*/
  void normalInterpretLogic();

  /* Special interpret logic is quite special as it's only used when the
   * player is currently performing an action command. Different 
   * logic may be also used depending on what type of ActionCommand is 
   * being used as well.*/
  void specialInterpretLogic();

  /* Only called during special interpret logic, and the player is using
   * the LightAttack. This is the basically the function that allows
   * the player to cancel the LightAttack into basically any other 
   * ActionCommand that the player has access except itself. That is, if
   * it lands.*/
  void lightAttackHandling();

  /* Only called during special interpret logic, and the player is using
   * the HeavyAttack. It's a bit more limited in what ActionCommands the
   * player could cancel into if the attack were to connect. Only allowing
   * the player to cancel into a Heavy Technique, but even so that may not
   * always be the case depending on the player's current sub-weapon.*/
  void heavyAttackHanding();

  /* The method retains the same functionality of the method it overrides
   * except it checks if the player is in low health afterwards.*/
  void takeDamage(uint16_t dmg_magnitude, float guard_pierce,
                  float stun_time, float kb_velocity = 0,
                  uint8_t kb_direction = 0) override;

  combatant_list *enemies;

  std::unique_ptr<SubWeapon> sub_weapon;

  uint8_t morale;
  uint8_t max_morale;

  bool moving;
  float movement_speed;

  bool critical_health = false;

  std::vector<uint8_t> input_buffer;
private:
  uint8_t *game_phase;

  bool buf_empty = false;
  bool buf_timer_started = false;

  float buf_clear_time;
  float buf_input_timestamp = 0;

  std::vector<int> anim_walk;
  float walk_frametime;

  float regen_time;
  float regen_timestamp = 0;

  bool moving_right = false;
  bool moving_left = false;
};
