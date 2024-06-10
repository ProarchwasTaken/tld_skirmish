// char_player.cpp
#include <memory>
#include <raylib.h>
#include "globals.h"
#include "utils.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "cmd_light_atk.h"
#include "cmd_heavy_atk.h"
#include "char_player.h"
#include <plog/Log.h>

using std::make_unique, std::unique_ptr;


PlayerCharacter::PlayerCharacter(combatant_list &enemies):
  Combatant("Player", TYPE_PLAYER, PLR_HP, PLR_START, PLR_HITBOX_SCALE, 
            {64, 64}, PLR_HITBOX_OFFSET)
{
  PLOGI << "Initializing the player character.";
  current_sprite = sprites::player[1];

  anim_walk = {1, 2, 3, 2};
  walk_frametime = 0.15;

  movement_speed = 1.75;

  buf_clear_time = 0.010;

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
      current_sprite = sprites::player[7];
      stunSequence();
      break;
    }
    case DEAD: {
      PLOGE << "The death sequence not implemented yet!";
      throw;
    }
    default: {
      commandSequence();
      interpretBuffer();
    }
  }

  clearBufferCheck();
}

bool PlayerCharacter::isMoving() {
  if (moving_left == moving_right) {
    current_sprite = sprites::player[1];
    return false;
  }
  else {
    Animation::play(this, sprites::player, anim_walk, walk_frametime);
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

  if (state == NEUTRAL) {
    PLOGI << "The player is in the NEUTRAL state. Using normal interpret" 
    " logic.";
    normalInterpretLogic();
  }
  else {
    PLOGI << "The player is not in the NEUTRAL state. Using special"
    " interpret logic based on the assumption that the player is using"
    " an action command.";
    specialInterpretLogic();
  }
}

void PlayerCharacter::normalInterpretLogic() {
  uint8_t first_input = input_buffer.front();
  unique_ptr<ActionCommand> command;

  PLOGI << "Deciding what commands to use depending on input buffer.";
  switch (first_input) {
    case BTN_LIGHT_ATK: {
      PLOGI << "Attempting to assign LightAttack";
      command = make_unique<LightAttack>(this);
      useCommand(command);
      break;
    }
    case BTN_HEAVY_ATK: {
      PLOGI << "Attempting to assign HeavyAttack";
      command = make_unique<HeavyAttack>(this);
      useCommand(command);
      break;
    }
    default: {
      PLOGI << "No valid commands found!";
      break;
    }
  }
}

void PlayerCharacter::specialInterpretLogic() {
  if (current_command == nullptr) {
    PLOGE << "Player doesn't have an command assigned to them!";
    throw;
  }

  PLOGI << "The player is using: " << current_command->command_name;
  if (current_command->command_name == "Light Attack") {
    lightAttackHandling();
    return;
  }
}

void PlayerCharacter::lightAttackHandling() {
  uint8_t first_input = input_buffer.front();
  unique_ptr<ActionCommand> command;

  auto light_atk = reinterpret_cast<LightAttack*>(current_command.get());

  PLOGI << "Deciding if the recovery phase should be canceled depending"
    " specific conditions.";
  if (light_atk->attack_connected && first_input == BTN_HEAVY_ATK) {
    PLOGI << "Canceling recovery phase and assigning HeavyAttack.";
    command = make_unique<HeavyAttack>(this);
    useCommand(command);
    return;
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

  if (offset <= -PLR_BOUNDS) {
    position.x = -PLR_BOUNDS + half_scaleX;
  }
  else if (offset >= PLR_BOUNDS) {
    position.x = PLR_BOUNDS - half_scaleX;
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
  bool key_v = IsKeyPressed(KEY_V);

  bool gamepad_available = IsGamepadAvailable(0);
  bool gamepad_right = false;
  bool gamepad_left = false;

  bool gamepad_face_right = false;
  bool gamepad_face_down = false;

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
    gamepad_face_down = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN
    );
  }


  bool input_right = key_right || (gamepad_available && gamepad_right);
  if (input_right && moving_right == false) {
    moving_right = true;
  }
  
  bool input_left = key_left || (gamepad_available && gamepad_left);
  if (input_left && moving_left == false) {
    moving_left = true;
  }

  bool input_light_attack = key_c || (gamepad_available && 
  gamepad_face_down);
  if (input_light_attack) {
    input_buffer.push_back(BTN_LIGHT_ATK);
  }

  bool input_heavy_attack = key_v || (gamepad_available && 
    gamepad_face_right);
  if (input_heavy_attack) {
    input_buffer.push_back(BTN_HEAVY_ATK);
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
  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  Rectangle dest = {tex_position.x, tex_position.y, 
    tex_scale.x, tex_scale.y};
  if (direction == LEFT) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, dest, {0, 0}, 0, WHITE);

  if (DEBUG_MODE == true) {
    drawDebug();
  }
}

void PlayerCharacter::drawDebug() {
  Actor::drawDebug();

  bool using_command;
  switch (state) {
    case NEUTRAL:
    case HIT_STUN:
    case DEAD: {
      using_command = false;
      break;
    }
    default: {
      using_command = true;
    }
  }

  if (using_command) {
    current_command->drawDebug();
  }
}
