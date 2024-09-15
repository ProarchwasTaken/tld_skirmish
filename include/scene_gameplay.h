// scene_gameplay.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include "base/generics.h"
#include "base/scene.h"
#include "game.h"
#include "sys_wave_manager.h"
#include "hud_life.h"
#include "hud_morale.h"
#include "char_player.h"

#define PHASE_REST 0
#define PHASE_ACTION 1


/* The most vital scene as it's where everything related to the main
 * gameplay loop will take place. (This is supposed to be a game btw.)
 * There are many important components that make up this scene.*/
class GameplayScene : public Scene {
public:
  GameplayScene(Game &skirmish, uint8_t weapon_id);
  ~GameplayScene() override;

  void tickTimer();
  uint8_t determinePhase();

  void pauseGame();
  void resumeGame();

  void drawTimer();
  void drawWaveCount();

  void checkInput() override;
  void checkPauseInput();

  void updateScene() override;

  void drawScene() override;
  void drawPauseMenu();
private:
  Texture background;
  Texture overlay;
  Color sky_color = BLACK;

  PlayerCharacter player = PlayerCharacter(enemies, phase);

  combatant_list enemies;
  dynamic_list dynamic_actors;

  LifeHud life_hud = LifeHud(player, phase);
  MoraleHud morale_hud = MoraleHud(player);

  WaveManager wave_manager = WaveManager(player, enemies);
  Camera2D camera;

  bool paused = false;
  float pause_timestamp = 0;

  uint8_t phase = PHASE_REST;
  uint8_t difficulty = 0;

  uint8_t max_wave;
  uint8_t wave = 0;

  uint16_t timer = 20;
  const float tick_interval = 1;
  float tick_timestamp = 0;

};
