// scenes/scene_debug.cpp
#include <raylib.h>
#include <functional>
#include <tuple>
#include <memory>
#include "globals.h"
#include "game.h"
#include "utils.h"
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
  life_hud = make_unique<LifeHud>(*player, phase);
  morale_hud = make_unique<MoraleHud>(*player);

  enemies = {
    make_shared<DummyEnemy>(*player, (Vector2){-96, 152}),
  };
  
  camera = CameraUtils::setupCamera();
  PLOGI << "Debug scene has loaded successfully!";
}

DebugScene::~DebugScene() {
  PLOGI << "Unloading Debug scene.";

  UnloadTexture(background);
  UnloadTexture(overlay);
  UnloadTexture(debug_overlay);

  player.reset();
  life_hud.reset();

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
  if (IsKeyPressed(KEY_E)) {
    enemies.push_back(
      make_shared<GhoulEnemy>(*player, (Vector2){450, 152})
    );
    PLOGD << "Spawned enemy at the right of the stage.";
  }

  if (IsKeyPressed(KEY_Q)) {
    enemies.push_back(
      make_shared<GhoulEnemy>(*player, (Vector2){-450, 152})
    );
    PLOGD << "Spawned enemy at the left side of the stage.";
  }

  if (IsKeyPressed(KEY_W)) {
    PLOGD << "Toggling game phase.";
    phase = !phase;
  }

  if (player->morale != 0 && IsKeyPressed(KEY_R)) {
    player->morale--;
  }
  if (player->morale != player->max_morale && IsKeyPressed(KEY_T)) {
    player->morale++;
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
      enemy->draw();
    }

    player->draw();

    for (auto &d_actor : dynamic_actors) {
      d_actor->draw();
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
  DrawTextEx(*fonts::skirmish, TextFormat("Game Phase: %i", phase), 
             {0, 40}, text_size, -3, GREEN);
}
