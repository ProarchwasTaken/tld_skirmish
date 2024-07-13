// scene_debug.h
#pragma once
#include <memory>
#include <functional>
#include <raylib.h>
#include "base/scene.h"
#include "base/generics.h"
#include "hud_life.h"
#include "char_player.h"

#define CAMERA_BOUNDS 512


/* The debug scene is meant to be a sandbox used for the testing of 
 * different game elements. It's not meant to be seen in normal play, and
 * it could only be accessed through the usage of a command line 
 * argument.*/
class DebugScene : public Scene {
public:
  DebugScene(std::function<void(int)> load_scene);
  ~DebugScene() override;

  void checkInput() override;
  void updateScene(double &delta_time) override;
  void drawScene() override;
  void drawDebugInfo() override;
private:
  Texture background;
  Texture overlay;
  Texture debug_overlay;

  Camera2D camera;

  std::shared_ptr<PlayerCharacter> player;
  std::unique_ptr<LifeHud> life_hud;
  combatant_list enemies;
};
