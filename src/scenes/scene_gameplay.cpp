// scene/scene_gameplay.cpp
#include <cstdint>
#include <raylib.h>
#include <functional>
#include <string>
#include <tuple>
#include <memory>
#include "base/combatant.h"
#include "globals.h"
#include "utils.h"
#include "game.h"
#include "sys_wave_manager.h"
#include "scene_gameplay.h"
#include <plog/Log.h>

using std::make_unique, std::tie, std::function, std::string;


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
  tick_timestamp = CURRENT_TIME;

  camera = CameraUtils::setupCamera();
  wave_manager = make_unique<WaveManager>(player, enemies);
  PLOGI << "Gameplay scene has loaded successfully!";
}

GameplayScene::~GameplayScene() {
  PLOGI << "Unloading gameplay scene.";
  UnloadTexture(background);
  UnloadTexture(overlay);

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  for (auto &d_actor : dynamic_actors) {
    d_actor.reset();
  }
  dynamic_actors.clear();

  wave_manager.reset();
  PLOGI << "Gameplay scene has unloaded successfully.";
}

void GameplayScene::checkInput() {
  bool valid_state = player.state != HIT_STUN && player.state != DEAD;
  bool valid_phase = phase == PHASE_REST || phase == PHASE_ACTION;

  bool can_pause = valid_state && valid_phase;
  if (can_pause) {
    checkPauseInput();
  }

  if (paused == false) {
    player.inputPressed();
  }

  player.inputReleased();
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

void GameplayScene::updateScene() {
  if (paused) {
    return;
  }

  tickTimer();

  player.update();
  CameraUtils::follow(camera, player.position.x);

  for (auto enemy : enemies) {
    enemy->update();
  }

  for (auto &d_actor : dynamic_actors) {
    d_actor->update();
  }

  life_hud.update();
  morale_hud.update();

  wave_manager->waveSequence();

  Dynamic::moveFromQueue(dynamic_actors);
  Dynamic::clearAwaitingDeletion(dynamic_actors);
  Enemies::deleteDeadEnemies(enemies);

  phase = determinePhase();

  // PLACEHOLDER
  if (player.awaiting_deletion) {
    PLOGW << "A proper fail state hasn't been implemented yet!";
    PLOGI << "Resorting to go back to the title screen for now.";
    load_scene(SCENE_TITLE);
  }
  else if (phase == PHASE_REST && wave == max_wave) {
    PLOGW << "A proper win state hasn't been implemented yet!";
    PLOGI << "Sorry for the inconvenience!";
    load_scene(SCENE_TITLE);
  }
}

void GameplayScene::pauseGame() {
  PLOGI << "Pausing the game.";
  paused = true;
  SoundUtils::pause();
  pause_timestamp = GetTime();
}

void GameplayScene::resumeGame() {
  PLOGI << "Resuming the game.";
  paused = false;

  PLOGD << "Current Time: " << GetTime();
  PLOGD << "Pause Timestamp: " << pause_timestamp;
  float time_paused = GetTime() - pause_timestamp;
  PLOGD << "Time Paused: " << time_paused;

  PAUSE_PENALTY += time_paused;
  SoundUtils::resume();
  PLOGD << "Pause Penalty: " << PAUSE_PENALTY;
}

void GameplayScene::tickTimer() {
  float time_elapsed = CURRENT_TIME - tick_timestamp;
  if (time_elapsed < tick_interval) {
    return;
  }

  tick_timestamp = CURRENT_TIME;
  
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
      enemy->draw(camera.target);
    }

    player.draw(camera.target);

    for (auto &d_actor : dynamic_actors) {
      d_actor->draw(camera.target);
    }

    DrawTexture(overlay, -512, 0, WHITE);
  }
  EndMode2D();

  life_hud.draw();
  morale_hud.draw();

  drawWaveCount();
  drawTimer();

  if (paused) {
    drawPauseMenu();
  }
}

// Placeholder
void GameplayScene::drawPauseMenu() {
  int size = fonts::skirmish->baseSize;
  std::string text = "PAUSED";

  Vector2 position = Text::alignCenter(fonts::skirmish, text, {213, 0}, 
                                       1, 0);

  DrawTextEx(*fonts::skirmish, text.c_str(), position, size, 0, WHITE);
}
