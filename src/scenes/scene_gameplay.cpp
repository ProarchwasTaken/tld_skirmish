// scene/scene_gameplay.cpp
#include <raylib.h>
#include <functional>
#include <tuple>
#include <memory>
#include "globals.h"
#include "utils.h"
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

  max_wave = 3;

  timer = 10;
  tick_interval = 1.0;
  tick_timestamp = GetTime();

  player = make_shared<PlayerCharacter>(enemies);
  life_hud = make_unique<LifeHud>(player.get());

  camera = CameraUtils::setupCamera();
  PLOGI << "Gameplay scene has loaded successfully!";
}

GameplayScene::~GameplayScene() {
  PLOGI << "Unloading gameplay scene.";
  UnloadTexture(background);
  UnloadTexture(overlay);

  player.reset();
  life_hud.reset();

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  PLOGI << "Gameplay scene has unloaded successfully.";
}

void GameplayScene::checkInput() {
  player->inputPressed();
  player->inputReleased();
}

void GameplayScene::updateScene(double &delta_time) {
  player->update(delta_time);
  CameraUtils::followPlayer(camera, *player, delta_time);

  for (auto enemy : enemies) {
    enemy->update(delta_time);
  }

  life_hud->update();
  Enemies::deleteDeadEnemies(enemies);

  tickTimer();
}

void GameplayScene::tickTimer() {
  float time_elapsed = GetTime() - tick_timestamp;
  if (time_elapsed < tick_interval) {
    return;
  }

  tick_timestamp = GetTime();
  
  if (timer != 0) {
    timer--;
    return;
  }
  else if (wave < max_wave) {
    PLOGI << "Proceeding to the next wave.";
    wave++;
  }

  if (wave != max_wave) {
    // This will probably change later.
    timer = 10;
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

  DrawTextEx(*fonts::skirmish, text, {0, 16}, size, 0, WHITE);
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
