// char_player.cpp
#include <memory>
#include <raylib.h>
#include "globals.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_light_atk.h"
#include "char_player.h"
#include <plog/Log.h>

using std::make_unique, std::unique_ptr;


PlayerCharacter::PlayerCharacter(combatant_list &enemies):
  Combatant("Player", TYPE_PLAYER, 100, {0, 208})
{
  PLOGI << "Initializing the player character.";
  movement_speed = 1.75;

  buf_clear_time = 0.025;

  PLOGI << "Assigning address to enemy list to pointer.";
  this->enemies = &enemies;
  PLOGI << "Player initialization complete.";
}

PlayerCharacter::~PlayerCharacter() {
  PLOGI << "Deleting the player character from memory.";
  input_buffer.clear();
}

void PlayerCharacter::update(double &delta_time) {
  bufferTimerCheck();

  switch (state) {
    case NEUTRAL: {
      moving = isMoving();
      movement(delta_time);

      interpretBuffer();
      break;
    }
    case HIT_STUN: {
      stunSequence();
      break;
    }
    default: {
      commandSequence();
    }
  }

  clearBufferCheck();
}

bool PlayerCharacter::isMoving() {
  if (moving_left == moving_right) {
    return false;
  }
  else {
    return true;
  }
}

void PlayerCharacter::bufferTimerCheck() {
  buf_empty = input_buffer.size() == 0;

  bool detected_first_input = !buf_empty && buf_timer_started == false;
  if (detected_first_input) {
    PLOGD << "Detected first input in the buffer. Starting timer.";
    buf_input_timestamp = GetTime();
    buf_timer_started = true;
  }
}

void PlayerCharacter::interpretBuffer() {
  if (buf_timer_started == false) {
    return;
  }

  float time_elapsed = GetTime() - buf_input_timestamp;
  bool about_to_clear = time_elapsed >= buf_clear_time;

  if (about_to_clear == false) {
    return;
  }

  PLOGD << "Interpreting input buffer...";
  int8_t first_input = input_buffer.front();
  unique_ptr<ActionCommand> command;

  PLOGI << "Deciding what commands to use depending on input buffer.";
  switch (first_input) {
    case BTN_LIGHT_ATK: {
      PLOGI << "Attempting to assign LightAttack";
      // Oh my god, what kind of black magic did I just do?
      Combatant *user = this;
      command = make_unique<LightAttack>(*user);
      useCommand(command);
      break;
    }
    default: {
      PLOGI << "No valid commands found!";
      break;
    }
  }
}

void PlayerCharacter::clearBufferCheck() {
  if (buf_timer_started == false) {
    return;
  }

  float time_elapsed = GetTime() - buf_input_timestamp;
  if (time_elapsed >= buf_clear_time) {
    PLOGD << "Clearing input buffer.";
    input_buffer.clear();
    buf_timer_started = false;
  }
}

void PlayerCharacter::movement(double &delta_time) {
  if (!moving) {
    return;
  }
  else if (moving_right) {
    direction = RIGHT;
  }
  else if (moving_left) {
    direction = LEFT;
  }

  float magnitude = (movement_speed * direction) * delta_time;
  int half_scaleX = hitbox_scale.x / 2;
  float offset = position.x + magnitude + (half_scaleX * direction);

  if (offset <= -PLAYER_BOUNDS) {
    position.x = -PLAYER_BOUNDS + half_scaleX;
  }
  else if (offset >= PLAYER_BOUNDS) {
    position.x = PLAYER_BOUNDS - half_scaleX;
  }
  else {
    position.x += magnitude;
  }

  hitboxCorrection();
  texRectCorrection();
}

void PlayerCharacter::inputPressed() {
  bool key_right = IsKeyPressed(KEY_RIGHT);
  bool key_left = IsKeyPressed(KEY_LEFT);

  bool key_c = IsKeyPressed(KEY_C);

  bool gamepad_available = IsGamepadAvailable(0);
  bool gamepad_right = false;
  bool gamepad_left = false;

  bool gamepad_face_right = false;

  if (gamepad_available) {
    gamepad_right = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT
    );
    gamepad_left = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_LEFT_FACE_LEFT
    );
    gamepad_face_right = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT
    );
  }

  bool input_right = key_right || (gamepad_available && gamepad_right);
  bool input_left = key_left || (gamepad_available && gamepad_left);
  bool input_light_attack = key_c || (gamepad_available && 
    gamepad_face_right);

  if (input_right && moving_right == false) {
    moving_right = true;
  }
  if (input_left && moving_left == false) {
    moving_left = true;
  }
  if (input_light_attack) {
    input_buffer.push_back(BTN_LIGHT_ATK);
  }

  // TODO: Remove this later!
  if (IsKeyPressed(KEY_E)) {
    takeDamage(5, 1);
  }
}

void PlayerCharacter::inputReleased() {
  bool key_right = IsKeyReleased(KEY_RIGHT);
  bool key_left = IsKeyReleased(KEY_LEFT);

  bool gamepad_available = IsGamepadAvailable(0);
  bool gamepad_right = false;
  bool gamepad_left = false;

  if (gamepad_available) {
    gamepad_right = IsGamepadButtonReleased(
      0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT
    );
    gamepad_left = IsGamepadButtonReleased(
      0, GAMEPAD_BUTTON_LEFT_FACE_LEFT
    ); 
  }

  bool input_right = key_right || (gamepad_available && gamepad_right);
  bool input_left = key_left || (gamepad_available && gamepad_left);

  if (input_right && moving_right) {
    moving_right = false;
  }
  if (input_left && moving_left) {
    moving_left = false;
  }
}

void PlayerCharacter::draw() {
  if (state == NEUTRAL) {
    DrawRectangleRec(hitbox, BLUE);
  }
  else {
    DrawRectangleRec(hitbox, ORANGE);
  }

  if (DEBUG_MODE) {
    drawDebug();
  }

  bool using_command = state != NEUTRAL && state != HIT_STUN;
  if (using_command) {
    current_command->draw();
    if (DEBUG_MODE) current_command->drawDebug();
  }
}
