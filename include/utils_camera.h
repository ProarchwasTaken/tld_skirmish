// utils_camera.h
#pragma once
#include <raylib.h>
#include "defaults.h"
#include "base/actor.h"


namespace CameraUtils {
  constexpr Vector2 offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  constexpr Vector2 target = {0, 120};

  constexpr Camera2D setupCamera() {
    return {offset, target, 0, 1};
  };

  /* For having the camera follow a specific x position if its outside
   * a certain threshold.*/
  void follow(Camera2D &camera, const float x_position, 
              const float max_speed);

  /* Returns true if the specified actor's tex rect on is on screen. This
   * function is mainly used to check if actor should be drawn.*/
  bool onScreen(const Actor *actor, const Vector2 &target);
}

