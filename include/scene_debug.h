// scene_debug.h
#pragma once
#include <memory>
#include <functional>
#include <raylib.h>
#include <cstdint>
#include "base/scene.h"
#include "base/generics.h"
#include "hud_life.h"
#include "hud_morale.h"
#include "char_player.h"


/* The same as the GameplayScene, but with anything related to the 
 * game loop being excluded, and some stuff that help with debugging The 
 * debug scene is meant to be a sandbox used for the testing of 
 * different game elements. It's not meant to be seen in normal play, and
 * it could only be accessed through the usage of a command line 
 * argument.*/
class DebugScene : public Scene {
public:
  DebugScene(std::function<void(int)> load_scene);
  ~DebugScene() override;

  void checkInput() override;
  void debugInputs();

  void updateScene(double &delta_time) override;

  void drawScene() override;
  void drawDebugInfo() override;
private:
  Texture background;
  Texture overlay;
  Texture debug_overlay;

  Camera2D camera;
  uint8_t phase;

  std::shared_ptr<PlayerCharacter> player;
  std::unique_ptr<LifeHud> life_hud;
  std::unique_ptr<MoraleHud> morale_hud;
  combatant_list enemies;
};
