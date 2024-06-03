// scene_debug.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include "defaults.h"
#include "scene_debug.h"
#include "char_player.h"
#include "enemy_dummy.h"
#include <plog/Log.h>

using std::make_shared;


DebugScene::DebugScene() {
  PLOGV << "Loading Debug scene.";
  test_room = LoadTexture("concept_art/test_room2.png");

  player = make_shared<PlayerCharacter>(enemies);
  enemies.push_back(make_shared<DummyEnemy>(*player, (Vector2){64, 208}));

  camera.target = {player->position.x, 120};
  camera.offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  camera.zoom = 1;
  camera.rotation = 0;

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

void DebugScene::cameraFollowPlayer(double &delta_time) {
  bool player_offCenter = camera.target.x != player->position.x;
  if (player_offCenter == false) {
    return;
  }

  float x_difference = player->position.x - camera.target.x;
  float next_x = x_difference / 8;
  int direction;

  if (x_difference > 0) {
    direction = 1;
  }
  else if (x_difference < 0) {
    direction = -1;
  }

  float boundary = CAMERA_BOUNDS * direction;
  float half_width = CANVAS_WIDTH / 2.0;
  float screen_side = camera.target.x + (half_width * direction);

  bool already_at_boundary = screen_side == boundary; 
  if (already_at_boundary) {
    return;
  }

  float offset = screen_side + next_x;

  bool hit_left_bounds = offset <= -CAMERA_BOUNDS;
  bool hit_right_bounds = offset >= CAMERA_BOUNDS;
  if (hit_left_bounds) {
    camera.target.x = -CAMERA_BOUNDS + half_width; 
  }
  else if (hit_right_bounds) {
    camera.target.x = CAMERA_BOUNDS - half_width;
  }
  else {
    camera.target.x += next_x * delta_time;
    if (FloatEquals(camera.target.x, player->position.x)) {
      camera.target.x = player->position.x;
    }
  }
}


void DebugScene::checkInput() {
  player->inputPressed();
  player->inputReleased();
}

void DebugScene::updateScene(double &delta_time) {
  player->update(delta_time);
  cameraFollowPlayer(delta_time);

  for (auto enemy : enemies) {
    enemy->update(delta_time);
  }
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
