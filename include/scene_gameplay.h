// scene_gameplay.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include "base/generics.h"
#include "base/scene.h"
#include "game.h"
#include "sys_wave_manager.h"
#include "utils_sequence.h"
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

  /* Basically the main function that makes the gameloop progress, and
   * it's logic is relatively simple at first glance. This decrements the
   * timer variable every second or so, and starts a new wave once it
   * hits zero. Resetting back to the default time of the started wave.
   * This continues until the final wave has been reached.*/
  void tickTimer();

  /* For determining what phase the game should be in depend on multiple
   * different factors. Capable of detecting if the current phase has
   * changed, and calls another function in response.*/
  uint8_t determinePhase();

  /* This function gets called every time the current phase changes. This
   * is for reseting and changing the color sequence so it matches the
   * phase given.*/
  void phaseChanged(const uint8_t new_phase);

  void pauseGame();
  void resumeGame();

  void drawTimer();
  void drawWaveCount();

  void checkInput() override;
  void checkPauseInput();

  void updateScene() override;

  /* Goes through the color sequence, assigning the current color to
   * sky_color. Only stopping when the end of the sequence has be reached
   * Ideally, this function should only be called after the phase has
   * been changed at least once, other than that, this function is called
   * once every frame.*/
  void phaseUpdate();

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
  
  Sequence<int, 3> seq_color = Sequence<int, 3>({0, 0, 0});

  bool paused = false;
  float pause_timestamp = 0;

  uint8_t phase = PHASE_REST;
  bool updated_phase = false;
  uint8_t difficulty = 0;

  uint8_t max_wave;
  uint8_t wave = 0;

  uint16_t timer = 20;
  const float tick_interval = 1;
  float tick_timestamp = 0;

};
