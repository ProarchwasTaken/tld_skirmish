// scene_debug.h
#pragma once
#include <memory>
#include <raylib.h>
#include "base/scene.h"
#include "base/generics.h"
#include "char_player.h"

#define CAMERA_BOUNDS 512


class DebugScene : public Scene {
public:
  DebugScene();
  ~DebugScene() override;

  void checkInput() override;
  void updateScene(double &delta_time) override;
  void drawScene() override;
  void drawDebugInfo() override;

  void cameraFollowPlayer(double &delta_time);
private:
  Texture test_room;
  Camera2D camera;

  std::shared_ptr<PlayerCharacter> player;
  combatant_list enemies;
};
