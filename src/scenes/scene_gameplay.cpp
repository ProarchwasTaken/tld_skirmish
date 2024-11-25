// scene/scene_gameplay.cpp
#include <cstdint>
#include <raylib.h>
#include <string>
#include <tuple>
#include "base/combatant.h"
#include "globals.h"
#include "utils_stages.h"
#include "utils_camera.h"
#include "utils_sound.h"
#include "utils_dynamic.h"
#include "utils_enemies.h"
#include "utils_text.h"
#include "utils_sequence.h"
#include "game.h"
#include "scene_gameplay.h"
#include "scene_title.h"
#include <plog/Log.h>

using std::tie, std::string;


GameplayScene::GameplayScene(Game &skirmish, uint8_t weapon_id): 
  Scene(skirmish)
{
  tie(background, overlay) = Stages::loadRandomStage();
  player.assignSubWeapon(weapon_id);

  max_wave = 3;
  sky_color = COLORS::PALETTE[40];

  camera = CameraUtils::setupCamera();

  skirmish.transition.fadein(1.5, BLACK);
  tick_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Gameplay scene.";
}

GameplayScene::~GameplayScene() {
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
  PLOGI << "Successfully unloaded Gameplay scene.";
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
  CameraUtils::follow(camera, player.camera_position, 2.5);

  for (auto enemy : enemies) {
    enemy->update();
  }

  for (auto &d_actor : dynamic_actors) {
    d_actor->update();
  }

  life_hud.update();
  morale_hud.update();

  wave_manager.waveSequence();

  Dynamic::moveFromQueue(dynamic_actors);
  Dynamic::clearAwaitingDeletion(dynamic_actors);
  Enemies::deleteDeadEnemies(enemies);

  phase = determinePhase();
  if (updated_phase) {
    phaseUpdate();
  }

  // PLACEHOLDER
  if (player.awaiting_deletion) {
    PLOGW << "A proper fail state hasn't been implemented yet!";
    PLOGI << "Resorting to go back to the title screen for now.";
    skirmish->loadScene<TitleScene>();
  }
  else if (phase == PHASE_REST && wave == max_wave) {
    PLOGW << "A proper win state hasn't been implemented yet!";
    PLOGI << "Sorry for the inconvenience!";
    skirmish->loadScene<TitleScene>();
  }
}

void GameplayScene::phaseUpdate() {
  if (seq_color.end_of_sequence) {
    return;
  }

  seq_color.play(0.25, false);
  sky_color = COLORS::PALETTE[*seq_color.iterator];
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
  if (skirmish->transition.active) {
    tick_timestamp = CURRENT_TIME;
    return;
  }

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

    wave_manager.startWave(difficulty);
    SoundUtils::play("wave_next");
  }

  if (wave != max_wave) {
    timer = wave_manager.wave_timer;
  }
}

uint8_t GameplayScene::determinePhase() {
  bool no_enemies = enemies.size() == 0;
  bool no_awaiting_spawn = wave_manager.enemy_queue.size() == 0;

  uint8_t old_phase = phase;
  uint8_t new_phase = 0;

  if (no_enemies && no_awaiting_spawn) {
    new_phase = PHASE_REST;
  }
  else {
    new_phase = PHASE_ACTION;
  }

  if (old_phase != new_phase) {
    phaseChanged(new_phase);
    return new_phase;
  }
  else {
    return old_phase;
  }
}

void GameplayScene::phaseChanged(const uint8_t new_phase) {
  PLOGD << "Phase has been changed to: " << int(new_phase);
  switch (new_phase) {
    case PHASE_REST: {
      seq_color.newSequence({32, 36, 40});
      break;
    }
    case PHASE_ACTION: {
      seq_color.newSequence({40, 36, 32});
      break;
    }
  }

  updated_phase = true;
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
  ClearBackground(sky_color);
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

  if (skirmish->transition.active == false) {
    life_hud.draw();
    morale_hud.draw();

    drawWaveCount();
    drawTimer();
  }

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
