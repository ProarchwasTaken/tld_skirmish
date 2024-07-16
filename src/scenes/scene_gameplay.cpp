// scene/scene_gameplay.cpp
#include <raylib.h>
#include <functional>
#include <tuple>
#include <memory>
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
}
