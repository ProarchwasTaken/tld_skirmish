// utils_camera.h
#pragma once
#include <raylib.h>
#include "base/actor.h"


namespace CameraUtils {
  Camera2D setupCamera();

  /* For having the camera follow a specific x position if its outside
   * a certain threshold.*/
  void follow(Camera2D &camera, float x_position);

  /* Returns true if the specified actor's tex rect on is on screen. This
   * function is mainly used to check if actor should be drawn.*/
  bool onScreen(Actor *actor, Vector2 &camera_target);
}

