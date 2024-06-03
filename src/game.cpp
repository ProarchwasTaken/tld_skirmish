// game.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include "defaults.h"
#include "game.h"
#include "globals.h"
#include "char_player.h"
#include "enemy_dummy.h"
#include <plog/Log.h>

using std::make_shared;


Game::Game() {
  setupCanvas();
  defineColorPalette();

  test_room = LoadTexture("concept_art/test_room2.png");

  player = make_shared<PlayerCharacter>(enemies);
  enemies.push_back(make_shared<DummyEnemy>(*player, (Vector2){64, 208}));

  camera.target = {player->position.x, 120};
  camera.offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  camera.zoom = 1;
  camera.rotation = 0;
}

Game::~Game() {
  PLOGV << "Request to terminate program detected.";
  UnloadRenderTexture(canvas);
  UnloadImagePalette(COLORS::PALETTE);

  UnloadTexture(test_room);
  player.reset();

  PLOGI << "Deleting all leftover enemies from memory.";
  for (auto enemy : enemies) {
    enemy.reset();
  }
  enemies.clear();
  PLOGV << "Thanks for playing!";
}

void Game::correctWindow() {
  PLOGI << "Attempting window correction.";
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  double canvas_ratio = canvas_dest.height / canvas_dest.width;

  canvas_dest.width = screen_width;

  if (IsWindowMaximized()) {
    canvas_dest.height = screen_height;
  }
  else { 
    double calulated_height = canvas_dest.width * canvas_ratio;
    canvas_dest.height = calulated_height;
  }

  bool lower_than_height = canvas_dest.height < screen_height;
  bool greater_than_height = canvas_dest.height > screen_height;

  if (lower_than_height || greater_than_height){
    SetWindowSize(screen_width, canvas_dest.height);
  }
}

void Game::setupCanvas() {
  PLOGI << "Setting up the canvas...";
  canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
  canvas_source = {0, 0, CANVAS_WIDTH, -CANVAS_HEIGHT};
  canvas_dest = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
}

void Game::defineColorPalette() {
  PLOGI << "Loading the game's color palette...";
  int color_count;
  Image palette = LoadImage("graphics/color_palette.png");

  COLORS::PALETTE = LoadImagePalette(palette, 56, &color_count);
  PLOGI << "Successfully loaded palette!";
  PLOGI << "Color Count: " << color_count;
  UnloadImage(palette);
}

void Game::cameraFollowPlayer(double &delta_time) {
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

void Game::refresh() {
  delta_time = GetFrameTime() * TARGET_FRAMERATE;
  player->inputPressed();
  player->inputReleased();

  player->update(delta_time);
  cameraFollowPlayer(delta_time);

  for (auto enemy : enemies) {
    enemy->update(delta_time);
  }

  BeginTextureMode(canvas);
  {
    BeginMode2D(camera);

    ClearBackground(BLACK);
    DrawTexture(test_room, -512, 0, WHITE);

    for (auto enemy : enemies) {
      enemy->draw();
    }

    player->draw();

    EndMode2D();
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);

    if (DEBUG_MODE) {
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
  }
  EndDrawing();
}
