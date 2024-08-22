// utils/camera.cpp
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include "defaults.h"
#include "globals.h"
#include "utils.h"


Camera2D CameraUtils::setupCamera() {
  Camera2D camera;
  camera.target = {0, 120};
  camera.offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  camera.zoom = 1;
  camera.rotation = 0;

  return camera;
}


void CameraUtils::follow(Camera2D &camera, float x_position) 
{
  bool off_center = std::abs(camera.target.x - x_position) > 0.01;
  if (off_center == false) {
    return;
  }

  float x_difference = x_position - camera.target.x;
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
    camera.target.x += next_x * DELTA_TIME;
  }
}

bool CameraUtils::onScreen(Actor *actor, Vector2 &camera_target) {
  Rectangle screen;
  screen.x = camera_target.x - (CANVAS_WIDTH / 2.0);
  screen.y = camera_target.y - (CANVAS_HEIGHT / 2.0);
  screen.width = CANVAS_WIDTH;
  screen.height = CANVAS_HEIGHT;

  if (CheckCollisionRecs(actor->tex_rect, screen)) {
    return true;
  }
  else {
    return false;
  }
}
