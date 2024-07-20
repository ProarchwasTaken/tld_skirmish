// scene_gameplay.h
#pragma once
#include <memory>
#include <functional>
#include <raylib.h>
#include <cstdint>
#include "base/generics.h"
#include "base/scene.h"
#include "wave_manager.h"
#include "hud_life.h"
#include "char_player.h"

#define PHASE_REST 0
#define PHASE_ACTION 1


/* The most vital scene as it's where everything related to the main
 * gameplay loop will take place. (This is supposed to be a game btw.)
 * There are many important components that make up this scene.*/
class GameplayScene : public Scene {
public:
  GameplayScene(std::function<void(int)> load_scene);
  ~GameplayScene() override;

  void tickTimer();
  uint8_t determinePhase();

  void pauseGame();
  void resumeGame();

  void drawTimer();
  void drawWaveCount();

  void checkInput() override;
  void checkPauseInput();
  void updateScene(double &delta_time) override;
  void drawScene() override;
private:
  Texture background;
  Texture overlay;

  std::shared_ptr<PlayerCharacter> player;
  combatant_list enemies;

  std::unique_ptr<WaveManager> wave_manager;
  std::unique_ptr<LifeHud> life_hud;
  Camera2D camera;

  bool paused = false;
  float pause_timestamp = 0;

  uint8_t phase;
  uint8_t difficulty;

  uint8_t max_wave;
  uint8_t wave = 0;

  uint16_t timer;
  float tick_interval;
  float tick_timestamp = 0;

};
