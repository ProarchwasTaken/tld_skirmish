// utils/camera.cpp
#include <raylib.h>
#include <raymath.h>
#include "defaults.h"
#include "utils.h"
#include "char_player.h"


Camera2D CameraUtils::setupCamera() {
  Camera2D camera;
  camera.target = {0, 120};
  camera.offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  camera.zoom = 1;
  camera.rotation = 0;

  return camera;
}


void CameraUtils::followPlayer(Camera2D &camera, PlayerCharacter &player,
                            double &delta_time) 
{
  bool player_offCenter = camera.target.x != player.position.x;
  if (player_offCenter == false) {
    return;
  }

  float x_difference = player.position.x - camera.target.x;
  float next_x = x_difference / 8;
  int direction;

  if (x_difference > 0) {
    direction = 1;
  }
  else if (x_difference < 0) {
    direction = -1;
  }

  float boundary = CAMERA_BOUNDS * direction;
  float half_width = CANVAS_WIDTH / 2.0;
  float screen_side = camera.target.x + (half_width * direction);

  bool already_at_boundary = screen_side == boundary; 
  if (already_at_boundary) {
    return;
  }

  float offset = screen_side + next_x;

  bool hit_left_bounds = offset <= -CAMERA_BOUNDS;
  bool hit_right_bounds = offset >= CAMERA_BOUNDS;
  if (hit_left_bounds) {
    camera.target.x = -CAMERA_BOUNDS + half_width; 
  }
  else if (hit_right_bounds) {
    camera.target.x = CAMERA_BOUNDS - half_width;
  }
  else {
    camera.target.x += next_x * delta_time;
    if (FloatEquals(camera.target.x, player.position.x)) {
      camera.target.x = player.position.x;
    }
  }
}
