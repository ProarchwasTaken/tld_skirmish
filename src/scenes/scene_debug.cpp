// scenes/scene_debug.cpp
#include <raylib.h>
#include <functional>
#include <tuple>
#include <memory>
#include "globals.h"
#include "game.h"
#include "utils.h"
#include "sys_wave_manager.h"
#include "scene_debug.h"
#include "scene_gameplay.h"
#include "char_player.h"
#include "hud_life.h"
#include "hud_morale.h"
#include "enemy_dummy.h"
#include "enemy_ghoul.h"
#include <plog/Log.h>

using std::make_shared, std::make_unique, std::function, std::tie;


DebugScene::DebugScene(function<void(int)> load_scene) : Scene(load_scene)
{
  PLOGI << "Loading Debug scene.";
  tie(background, overlay) = Stages::loadStage("debug");
  debug_overlay = LoadTexture("graphics/stages/debug/debug_overlay.png");

  phase = PHASE_ACTION;

  player = make_shared<PlayerCharacter>(enemies, phase);
  enemies = {
    make_shared<DummyEnemy>(*player, (Vector2){-96, 152}),
  };
  
  life_hud = make_unique<LifeHud>(*player, phase);
  morale_hud = make_unique<MoraleHud>(*player);

  camera = CameraUtils::setupCamera();
  wave_manager = make_unique<WaveManager>(*player, enemies);
  PLOGI << "Debug scene has loaded successfully!";
}

DebugScene::~DebugScene() {
  PLOGI << "Unloading Debug scene.";
  UnloadTexture(background);
  UnloadTexture(overlay);
  UnloadTexture(debug_overlay);

  life_hud.reset();
  morale_hud.reset();
  wave_manager.reset();

  player.reset();

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  for (auto &d_actor : dynamic_actors) {
    d_actor.reset();
  }
  dynamic_actors.clear();

  PLOGI << "Debug scene has unloaded succesfully.";
}

void DebugScene::checkInput() {
  debugInputs();

  player->inputPressed();
  player->inputReleased();
}

void DebugScene::debugInputs() {
  if (IsKeyPressed(KEY_Q)) {
    PLOGD << "Toggling game phase.";
    phase = !phase;
  }

  if (player->morale != 0 && IsKeyPressed(KEY_W)) {
    player->morale--;
  }
  if (player->morale != player->max_morale && IsKeyPressed(KEY_E)) {
    player->morale++;
  }

  if (IsKeyPressed(KEY_R) && wave_manager->enemy_queue.size() == 0) {
    wave_manager->startWaveByID(0);
  }
}

void DebugScene::updateScene() {
  player->update();
  CameraUtils::followPlayer(camera, *player);

  for (auto enemy : enemies) {
    enemy->update();
  }

  for (auto &d_actor : dynamic_actors) {
    d_actor->update();
  }

  life_hud->update();
  morale_hud->update();
  wave_manager->waveSequence();

  Dynamic::moveFromQueue(dynamic_actors);
  Dynamic::clearAwaitingDeletion(dynamic_actors);
  Enemies::deleteDeadEnemies(enemies);

  if (player->awaiting_deletion) {
    PLOGD << "Reloading scene...";
    load_scene(SCENE_DEBUG);
  }
}

void DebugScene::drawScene() {
  BeginMode2D(camera); 
  {
    DrawTexture(background, -512, 0, WHITE);
    
    for (auto enemy : enemies) {
      enemy->draw(camera.target);
    }

    player->draw(camera.target);

    for (auto &d_actor : dynamic_actors) {
      d_actor->draw(camera.target);
    }

    DrawTexture(overlay, -512, 0, WHITE);

    if (DEBUG_MODE) {
      DrawTexture(debug_overlay, -512, 0, WHITE);
    }
  }
  EndMode2D();

  life_hud->draw();
  morale_hud->draw();
}

void DebugScene::drawDebugInfo() {
  int text_size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, TextFormat("FPS: %i", GetFPS()), {0, 0}, 
             text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, TextFormat("Position: (%03.02f, %03.02f)", 
                                          player->position.x, 
                                          player->position.y), 
             {0, 8}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, 
             TextFormat("Enemy Queue: %i",
                        wave_manager->enemy_queue.size()),
             {0, 16}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, 
             TextFormat("Enemies Active: %i", enemies.size()), 
             {0, 24}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, 
             TextFormat("D Actors: %i", dynamic_actors.size()),
             {0, 32}, text_size, -3, GREEN);
}
