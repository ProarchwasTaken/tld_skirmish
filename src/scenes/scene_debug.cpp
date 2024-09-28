// scenes/scene_debug.cpp
#include <cctype>
#include <raylib.h>
#include <string>
#include <tuple>
#include <memory>
#include "globals.h"
#include "game.h"
#include "utils_stages.h"
#include "utils_camera.h"
#include "utils_enemies.h"
#include "utils_dynamic.h"
#include "utils_text.h"
#include "enemy_dummy.h"
#include "scene_gameplay.h"
#include "scene_debug.h"
#include <plog/Log.h>

using std::make_shared, std::tie;


DebugScene::DebugScene(Game &skirmish) : Scene(skirmish)
{
  tie(background, overlay) = Stages::loadStage("debug");
  debug_overlay = LoadTexture("graphics/stages/debug/debug_overlay.png");

  phase = PHASE_ACTION;

  enemies = {
    make_shared<DummyEnemy>(player, (Vector2){-96, 152}),
  };
  
  camera = CameraUtils::setupCamera();
  player.assignSubWeapon(WEAPON_KNIFE);
  PLOGI << "Loaded Debug scene.";
}

DebugScene::~DebugScene() {
  UnloadTexture(background);
  UnloadTexture(overlay);
  UnloadTexture(debug_overlay);

  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();

  for (auto &d_actor : dynamic_actors) {
    d_actor.reset();
  }
  dynamic_actors.clear();

  num_buffer.clear();
  PLOGI << "Successfully unloaded Debug scene.";
}

void DebugScene::checkInput() {
  debugInputs();

  player.inputPressed();
  player.inputReleased();
}

void DebugScene::debugInputs() {
  int unicode = GetKeyPressed();
  if (unicode != 0) {
    appendNumBuffer(unicode);
  }

  if (IsKeyPressed(KEY_Q)) {
    PLOGD << "Toggling game phase.";
    phase = !phase;
  }

  if (player.morale != 0 && IsKeyPressed(KEY_W)) {
    player.morale--;
  }
  if (player.morale != player.max_morale && IsKeyPressed(KEY_E)) {
    player.morale++;
  }

  if (IsKeyPressed(KEY_R)) {
    wave_manager.reloadWaveData();
  }

  if (IsKeyPressed(KEY_K)) {
    player.takeDamage(19, 0.0, 0.2);
  }

  if (num_buffer.size() == 0) {
    return;
  }

  if (IsKeyPressed(KEY_BACKSPACE)) {
    num_buffer.pop_back();
  }

  bool no_awaiting_spawn = wave_manager.enemy_queue.size() == 0;
  if (IsKeyPressed(KEY_ENTER) && no_awaiting_spawn) {
    int wave_id = std::stoi(num_buffer);
    wave_manager.startWaveByID(wave_id);
    num_buffer.clear();
  }

  if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
    int weapon_id = std::stoi(num_buffer);
    player.assignSubWeapon(weapon_id);
    num_buffer.clear();
  }
}

void DebugScene::appendNumBuffer(int unicode) {
  char key_pressed = static_cast<char>(unicode);

  if (isdigit(key_pressed)) {
    num_buffer.push_back(key_pressed);
  }
}

void DebugScene::updateScene() {
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
  wave_manager.waveSequence();

  Dynamic::moveFromQueue(dynamic_actors);
  Dynamic::clearAwaitingDeletion(dynamic_actors);
  Enemies::deleteDeadEnemies(enemies);

  if (player.awaiting_deletion) {
    PLOGD << "Reloading scene...";
    skirmish->loadScene<DebugScene>();
  }
}

void DebugScene::drawScene() {
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

    if (DEBUG_MODE) {
      DrawTexture(debug_overlay, -512, 0, WHITE);
    }
  }
  EndMode2D();

  life_hud.draw();
  morale_hud.draw();

  drawNumBuffer();
}

void DebugScene::drawDebugInfo() {
  int text_size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, TextFormat("FPS: %i", GetFPS()), {0, 0}, 
             text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, TextFormat("Position: (%03.02f, %03.02f)", 
                                          player.position.x, 
                                          player.position.y), 
             {0, 8}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, 
             TextFormat("Enemy Queue: %i",
                        wave_manager.enemy_queue.size()),
             {0, 16}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, 
             TextFormat("Enemies Active: %i", enemies.size()), 
             {0, 24}, text_size, -3, GREEN);
  DrawTextEx(*fonts::skirmish, 
             TextFormat("D Actors: %i", dynamic_actors.size()),
             {0, 32}, text_size, -3, GREEN);

  bool off_center = camera.target.x != player.position.x;
  DrawTextEx(*fonts::skirmish, TextFormat("Off Center: %i", off_center),
             {0, 40}, text_size, -3, GREEN);

  DrawTextEx(*fonts::skirmish, TextFormat("Combo: %i", player.combo), 
             {0, 48}, text_size, -3, GREEN);
}

void DebugScene::drawNumBuffer() {
  int size = fonts::skirmish->baseSize;
  Vector2 position = Text::alignRight(fonts::skirmish, num_buffer, 
                                      {426, 0}, 1, -3);
  DrawTextEx(*fonts::skirmish, num_buffer.c_str(), position, size, -3, 
             RED);
}
