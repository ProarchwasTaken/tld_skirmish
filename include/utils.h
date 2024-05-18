// utils.h
#pragma once
#include <raylib.h>
#include "char_player.h"

#define CAMERA_BOUNDS 512


namespace CameraUtils {
  Camera2D setupCamera();

  /* For having the camera follow the player whenever they are off center.
   * This process only stops when the player is at the center of the 
   * screen again, or the camera has hit a boundary.*/
  void followPlayer(Camera2D &camera, PlayerCharacter &player, 
                    double &delta_time);
}
