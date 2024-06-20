// scene_debug.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include "utils.h"
#include "scene_debug.h"
#include "char_player.h"
#include "enemy_dummy.h"
#include "enemy_ghoul.h"
#include <plog/Log.h>

using std::make_shared;


DebugScene::DebugScene() {
  PLOGV << "Loading Debug scene.";
  test_room = LoadTexture("concept_art/test_room2.png");

  player = make_shared<PlayerCharacter>(enemies);

  enemies = {
    make_shared<DummyEnemy>(*player, (Vector2){-96, 208}),
  };
  
  camera = CameraUtils::setupCamera();
  PLOGV << "Debug scene has loaded successfully!";
}

DebugScene::~DebugScene() {
  PLOGV << "Unloading Debug scene.";

  UnloadTexture(test_room);
  player.reset();

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  PLOGV << "Debug scene has unloaded succesfully.";
}

void DebugScene::checkInput() {
  player->inputPressed();
  player->inputReleased();
}

void DebugScene::updateScene(double &delta_time) {
  if (IsKeyPressed(KEY_E)) {
    enemies.push_back(
      make_shared<GhoulEnemy>(*player, (Vector2){450, 208})
    );
  }
  if (IsKeyPressed(KEY_Q)) {
    enemies.push_back(
      make_shared<GhoulEnemy>(*player, (Vector2){-450, 208})
    );
  }

  player->update(delta_time);
  CameraUtils::followPlayer(camera, *player, delta_time);

  for (auto enemy : enemies) {
    enemy->update(delta_time);
  }

  Enemies::deleteDeadEnemies(enemies);
}

void DebugScene::drawScene() {
  BeginMode2D(camera); 
  {
    DrawTexture(test_room, -512, 0, WHITE);
    
    for (auto enemy : enemies) {
      enemy->draw();
    }

    player->draw();
  }
  EndMode2D();
}

void DebugScene::drawDebugInfo() {
  DrawText(TextFormat("FPS: %i", GetFPS()), 16, 16, 32, PURPLE);
  DrawText(TextFormat("Player Position: (%02.02f, %02.02f)",
                      player->position.x, player->position.y), 
           16, 48, 32, PURPLE);
  DrawText(TextFormat("Player Health: %i", player->health), 
           16, 80, 32, PURPLE);
  DrawText(TextFormat("Player State: %i", player->state), 
           16, 112, 32, PURPLE);
  DrawText(TextFormat("Camera X Difference: %f", 
                      player->position.x - camera.target.x), 
           16, 144, 32, PURPLE);
}
