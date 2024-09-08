// utils/camera.cpp
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include "defaults.h"
#include "globals.h"
#include "utils_camera.h"


void CameraUtils::follow(Camera2D &camera, const float x_position) 
{
  bool off_center = std::abs(camera.target.x - x_position) > 0.01;
  if (off_center == false) {
    return;
  }

  const float x_difference = x_position - camera.target.x;
  const float next_x = x_difference / 8;
  int direction;

  if (x_difference > 0) {
    direction = 1;
  }
  else if (x_difference < 0) {
    direction = -1;
  }

  const float boundary = CAMERA_BOUNDS * direction;
  const float half_width = CANVAS_WIDTH / 2.0;
  const float screen_side = camera.target.x + (half_width * direction);

  bool already_at_boundary = screen_side == boundary; 
  if (already_at_boundary) {
    return;
  }

  const float offset = screen_side + next_x;

  bool hit_left_bounds = offset <= -CAMERA_BOUNDS;
  bool hit_right_bounds = offset >= CAMERA_BOUNDS;
  if (hit_left_bounds || hit_right_bounds) {
    camera.target.x = (CAMERA_BOUNDS - half_width) * direction; 
  }
  else {
    camera.target.x += next_x * DELTA_TIME;
  }
}

bool CameraUtils::onScreen(const Actor *actor, const Vector2 &target) {
  Rectangle screen;
  screen.x = target.x - (CANVAS_WIDTH / 2.0);
  screen.y = target.y - (CANVAS_HEIGHT / 2.0);
  screen.width = CANVAS_WIDTH;
  screen.height = CANVAS_HEIGHT;

  if (CheckCollisionRecs(actor->tex_rect, screen)) {
    return true;
  }
  else {
    return false;
  }
}
