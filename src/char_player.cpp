// char_player.cpp
#include <raylib.h>
#include "char_player.h"
#include <plog/Log.h>


PlayerCharacter::PlayerCharacter() {
  PLOGI << "Initializing the player character.";
  position = {0, 208};
  hitbox_scale = {32, 64};
  tex_scale = {64, 64};

  movement_speed = 1.5;
  direction = RIGHT;

  hitboxCorrection();
  texRectCorrection();
  PLOGI << "Player initialization complete.";
}

void PlayerCharacter::hitboxCorrection() {
  float x = position.x - hitbox_scale.x / 2;
  float y = position.y - hitbox_scale.y;

  hitbox_position = {x, y};
  hitbox = {x, y, hitbox_scale.x, hitbox_scale.y};
}

void PlayerCharacter::texRectCorrection() {
  float x = position.x - tex_scale.x / 2;
  float y = position.y - tex_scale.y;

  tex_position = {x, y};
  tex_rect = {x, y, tex_scale.x, tex_scale.y};
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
  DrawRectangleLinesEx(tex_rect, 1, BLUE);
  DrawRectangleLinesEx(hitbox, 2, RED);
}
