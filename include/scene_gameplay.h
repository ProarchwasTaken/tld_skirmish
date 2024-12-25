// scene_gameplay.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include "base/generics.h"
#include "base/scene.h"
#include "game.h"
#include "sys_wave_manager.h"
#include "sys_transition.h"
#include "utils_sequence.h"
#include "hud_life.h"
#include "hud_morale.h"
#include "hud_pause.h"
#include "char_player.h"

#define PHASE_REST 0
#define PHASE_ACTION 1
#define PHASE_WIN 2
#define PHASE_LOSE 3


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

  /* Updates everything entity that derives from the Actor class. This
   * includes Combatants, DynamicActors, and the PlayerCharacter.*/
  void updateActors();

  /* Pretty much updates everything else that's vital for the game 
   * scene. Think of it like miscellaneous update method.*/
  void systemUpdate();

  void updateGameState();

  /* Goes through the color sequence, assigning the current color to
   * sky_color. Only stopping when the end of the sequence has be reached
   * Ideally, this function should only be called after the phase has
   * been changed at least once, other than that, this function is called
   * once every frame.*/
  void skyColorUpdate();

  /* For determining if the game should end in a lose or a win depending
   * on certain conditions.*/
  void endGameCheck();
  void endGameProcedures();

  /* When the player wins the game, there is a small delay before 
   * control is taken away from them and the win sequence properly 
   * starts*/
  void winDelay();
  void winSequence();

  void loseDelay();
  void loseSequence();

  void drawScene() override;
  void drawBgTransition();
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
  PauseHud pause_hud = PauseHud(this);

  WaveManager wave_manager = WaveManager(player, enemies);
  Camera2D camera;
  
  Sequence<int, 3> seq_color = Sequence<int, 3>({0, 0, 0});

  WipeTransition bg_transition;
  bool fading_out = false;

  bool paused = false;
  float pause_timestamp = 0;

  uint8_t phase = PHASE_REST;
  bool updated_phase = false;
  uint8_t difficulty = 0;

  bool awaiting_win = false;
  bool awaiting_lose = false;

  float win_delay = 3.0;
  float win_time = 8.5;

  float lose_delay = 0.5;
  float lose_time = 3.0;

  float end_prevtime = 0.0;

  uint8_t max_wave;
  uint8_t wave = 0;

  uint16_t timer = 20;
  const float tick_interval = 1;
  float tick_timestamp = 0;
};
