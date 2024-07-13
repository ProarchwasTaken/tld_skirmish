// scenes/scene_debug.cpp
#include <raylib.h>
#include <functional>
#include <raymath.h>
#include <memory>
#include "globals.h"
#include "utils.h"
#include "base/scene.h"
#include "scene_debug.h"
#include "char_player.h"
#include "hud_life.h"
#include "enemy_dummy.h"
#include "enemy_ghoul.h"
#include <plog/Log.h>

using std::make_shared, std::make_unique, std::function;


DebugScene::DebugScene(function<void(int)> load_scene) : Scene(load_scene)
{
  PLOGI << "Loading Debug scene.";
  test_room = LoadTexture("concept_art/test_room2.png");

  player = make_shared<PlayerCharacter>(enemies);
  life_hud = make_unique<LifeHud>(player.get());

  enemies = {
    make_shared<DummyEnemy>(*player, (Vector2){-96, 208}),
  };
  
  camera = CameraUtils::setupCamera();
  PLOGI << "Debug scene has loaded successfully!";
}

DebugScene::~DebugScene() {
  PLOGI << "Unloading Debug scene.";

  UnloadTexture(test_room);
  player.reset();

  life_hud.reset();

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  PLOGI << "Debug scene has unloaded succesfully.";
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

  life_hud->update();
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

  life_hud->draw();
}

void DebugScene::drawDebugInfo() {
  int text_size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, TextFormat("FPS: %i", GetFPS()), {0, 0}, 
             text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, TextFormat("State: %i", player->state), 
             {0, 8}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, TextFormat("Position: (%03.02f, %03.02f)", 
                                          player->position.x, 
                                          player->position.y), 
             {0, 16}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, TextFormat("Invulnerable: %i", 
                                          player->invulnerable), 
             {0, 24}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, TextFormat("Parry: %i",
                                          player->parried_attack), 
             {0, 32}, text_size, -3, GREEN);
}
