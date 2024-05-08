// char_player.cpp
#include <raylib.h>
#include "globals.h"
#include "base/combatant.h"
#include "char_player.h"
#include <plog/Log.h>


PlayerCharacter::PlayerCharacter():
  Combatant("Player", TYPE_PLAYER, 100, {0, 208})
{
  PLOGI << "Initializing the player character.";
  movement_speed = 1.5;
  direction = RIGHT;

  PLOGI << "Player initialization complete.";
}

void PlayerCharacter::update(double &delta_time) {
  moving = isMoving();
  movement(delta_time);
}

bool PlayerCharacter::isMoving() {
  if (moving_left && moving_right) {
    return false;
  }
  else if (!moving_left && !moving_right) {
    return false;
  }
  else {
    return true;
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

  bool gamepad_available = IsGamepadAvailable(0);
  bool gamepad_right = false;
  bool gamepad_left = false;

  if (gamepad_available) {
    gamepad_right = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT
    );
    gamepad_left = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_LEFT_FACE_LEFT
    );
  }

  bool input_right = key_right || (gamepad_available && gamepad_right);
  bool input_left = key_left || (gamepad_available && gamepad_left);

  if (input_right && moving_right == false) {
    moving_right = true;
  }
  if (input_left && moving_left == false) {
    moving_left = true;
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
  DrawRectangleRec(hitbox, BLUE);

  if (DEBUG_MODE) {
    debugDraw();
  }
}

void PlayerCharacter::debugDraw() {
  DrawRectangleLinesEx(tex_rect, 1, YELLOW);
  DrawRectangleLinesEx(hitbox, 1, RED);
}
