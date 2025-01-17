// scene_debug.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include <string>
#include <triblib/base/scene.h>
#include <triblib/base/generics.h>
#include "game.h"
#include "system/wave_manager.h"
#include "hud/hud_life.h"
#include "hud/hud_morale.h"
#include "combatants/char_player.h"


/* The same as the GameplayScene, but with anything related to the 
 * game loop being excluded, and some stuff that help with debugging The 
 * debug scene is meant to be a sandbox used for the testing of 
 * different game elements. It's not meant to be seen in normal play, and
 * it could only be accessed through the usage of a command line 
 * argument.*/
class DebugScene : public Scene {
public:
  DebugScene(Game &skirmish);
  ~DebugScene() override;

  void checkInput() override;
  void debugInputs();

  void updateScene() override;

  void appendNumBuffer(int unicode);
  void drawNumBuffer();

  void drawScene() override;
  void drawDebugInfo() override;
private:
  Texture background;
  Texture overlay;
  Texture debug_overlay;

  Camera2D camera;
  uint8_t phase;

  PlayerCharacter player = PlayerCharacter(enemies, phase);
  combatant_list enemies;
  dynamic_list dynamic_actors;

  LifeHud life_hud = LifeHud(player, phase);
  MoraleHud morale_hud = MoraleHud(player);

  WaveManager wave_manager = WaveManager(player, enemies);
  std::string num_buffer;
};
