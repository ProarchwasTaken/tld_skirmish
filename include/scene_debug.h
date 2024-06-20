// scene_debug.h
#pragma once
#include <memory>
#include <raylib.h>
#include "base/scene.h"
#include "base/generics.h"
#include "char_player.h"

#define CAMERA_BOUNDS 512


/* The debug scene is meant to be a sandbox used for the testing of 
 * different game elements. It's not meant to be seen in normal play, and
 * it could only be accessed through the usage of a command line 
 * argument.*/
class DebugScene : public Scene {
public:
  DebugScene();
  ~DebugScene() override;

  void checkInput() override;
  void updateScene(double &delta_time) override;
  void drawScene() override;
  void drawDebugInfo() override;
private:
  Texture test_room;
  Camera2D camera;

  std::shared_ptr<PlayerCharacter> player;
  combatant_list enemies;
};
