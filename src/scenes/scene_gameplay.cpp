// scene/scene_gameplay.cpp
#include <cstdint>
#include <raylib.h>
#include <functional>
#include <tuple>
#include <memory>
#include "base/combatant.h"
#include "globals.h"
#include "utils.h"
#include "wave_manager.h"
#include "char_player.h"
#include "hud_life.h"
#include "scene_gameplay.h"
#include <plog/Log.h>

using std::make_shared, std::make_unique, std::tie, std::function;


GameplayScene::GameplayScene(function<void(int)> load_scene):
Scene(load_scene)
{
  PLOGI << "Loading Gameplay scene.";
  tie(background, overlay) = Stages::loadStage("arisen");

  phase = PHASE_REST;

  max_wave = 3;
  difficulty = 0;

  timer = 20;
  tick_interval = 1;
  tick_timestamp = GetTime();

  player = make_shared<PlayerCharacter>(enemies, phase);
  life_hud = make_unique<LifeHud>(*player, phase);

  camera = CameraUtils::setupCamera();
  wave_manager = make_unique<WaveManager>(*player, enemies);
  PLOGI << "Gameplay scene has loaded successfully!";
}

GameplayScene::~GameplayScene() {
  PLOGI << "Unloading gameplay scene.";
  UnloadTexture(background);
  UnloadTexture(overlay);

  player.reset();
  life_hud.reset();
  wave_manager.reset();

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  PLOGI << "Gameplay scene has unloaded successfully.";
}

void GameplayScene::checkInput() {
  bool valid_state = player->state != HIT_STUN && player->state != DEAD;
  bool valid_phase = phase == PHASE_REST || phase == PHASE_ACTION;

  bool can_pause = valid_state && valid_phase;
  if (can_pause) {
    checkPauseInput();
  }

  if (paused == false) {
    player->inputPressed();
  }

  player->inputReleased();
}

void GameplayScene::checkPauseInput() {
  bool key_tab = IsKeyPressed(KEY_TAB);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_start = false;

  if (gamepad_detected) {
    btn_start = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
  }

  if (key_tab || btn_start) {
    if (paused == false) pauseGame();
    else resumeGame();
  }
}

void GameplayScene::updateScene(double &delta_time) {
  if (paused) {
    return;
  }

  tickTimer();

  player->update(delta_time);
  CameraUtils::followPlayer(camera, *player, delta_time);

  for (auto enemy : enemies) {
    enemy->update(delta_time);
  }

  life_hud->update();

  wave_manager->waveSequence();
  Enemies::deleteDeadEnemies(enemies);
  phase = determinePhase();
}

void GameplayScene::pauseGame() {
  PLOGI << "Pausing the game.";
  paused = true;
  pause_timestamp = GetTime();
}

void GameplayScene::resumeGame() {
  PLOGI << "Resuming the game.";
  paused = false;

  double time_paused = GetTime() - pause_timestamp;
  PLOGD << "Time Paused: " << time_paused;

  PAUSE_PENALTY += time_paused;
  PLOGD << "Pause Penalty: " << PAUSE_PENALTY;
}

void GameplayScene::tickTimer() {
  float time_elapsed = GetTime() - tick_timestamp;
  if (time_elapsed < tick_interval) {
    return;
  }

  tick_timestamp = GetTime();
  
  if (timer != 0) {
    timer--;
    if (timer <= 5) SoundUtils::play("timer_low");
    return;
  }
  else if (wave < max_wave) {
    PLOGI << "Proceeding to the next wave.";
    wave++;
    difficulty++;

    wave_manager->startWave(difficulty);
    SoundUtils::play("wave_next");
  }

  if (wave != max_wave) {
    timer = wave_manager->wave_timer;
  }
}

uint8_t GameplayScene::determinePhase() {
  bool no_enemies = enemies.size() == 0;
  bool no_awaiting_spawn = wave_manager->enemy_queue.size() == 0;

  if (no_enemies && no_awaiting_spawn) {
    return PHASE_REST;
  }
  else {
    return PHASE_ACTION;
  }
}

void GameplayScene::drawWaveCount() {
  const char *text = TextFormat("WAVE - %i/%i", wave, max_wave);
  int size = fonts::skirmish->baseSize;

  DrawTextEx(*fonts::skirmish, text, {0, 0}, size, 0, WHITE);
}

void GameplayScene::drawTimer() {
  const char *text = TextFormat("TIME - %i", timer);
  int size = fonts::skirmish->baseSize;

  DrawTextEx(*fonts::skirmish, text, {0, 10}, size, 0, WHITE);
}


void GameplayScene::drawScene() {
  BeginMode2D(camera);
  {
    DrawTexture(background, -512, 0, WHITE);

    for (auto enemy : enemies) {
      enemy->draw();
    }

    player->draw();

    DrawTexture(overlay, -512, 0, WHITE);
  }
  EndMode2D();

  life_hud->draw();

  drawWaveCount();
  drawTimer();
}
