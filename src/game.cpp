// game.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include "defaults.h"
#include "game.h"
#include "globals.h"
#include "char_player.h"
#include <plog/Log.h>

using std::make_unique;


Game::Game() {
  setupCanvas();
  defineColorPalette();

  test_room = LoadTexture("concept_art/test_room2.png");

  player = make_unique<PlayerCharacter>();
  camera.target = {player->position.x, 120};
  camera.offset = {CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0};
  camera.zoom = 1;
  camera.rotation = 0;

  camera_boundX = 512;
}

Game::~Game() {
  PLOGV << "Request to terminate program detected.";
  UnloadRenderTexture(canvas);
  UnloadImagePalette(COLORS::PALETTE);

  UnloadTexture(test_room);
  player.reset();
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

void Game::cameraFollowPlayer() {
  bool player_offCenter = camera.target.x != player->position.x;
  if (player_offCenter == false) {
    return;
  }

  float x_difference = player->position.x - camera.target.x;
  float direction = Clamp(x_difference, -1, 1);

  float boundary = camera_boundX * direction;
  int half_width = CANVAS_WIDTH / 2;
  float screen_side = camera.target.x + (half_width * direction);

  bool already_at_boundary = screen_side == boundary; 
  if (already_at_boundary) {
    return;
  }

  float offset = screen_side + x_difference;

  bool hit_left_bounds = direction < 0 && offset <= boundary;
  if (hit_left_bounds) {
    camera.target = {boundary + half_width, 120}; 
    return;
  }

  bool hit_right_bounds = direction > 0 && offset > boundary;
  if (hit_right_bounds) {
    camera.target = {boundary - half_width, 120};
    return;
  }

  camera.target = {player->position.x, 120};
}

void Game::refresh() {
  delta_time = GetFrameTime() * TARGET_FRAMERATE;
  player->inputPressed();
  player->inputReleased();

  player->update(delta_time);
  cameraFollowPlayer();

  BeginTextureMode(canvas);
  {
    BeginMode2D(camera);

    ClearBackground(BLACK);
    DrawTexture(test_room, -512, 0, WHITE);
    player->draw();

    EndMode2D();
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);

    if (DEBUG_MODE) {
      DrawText(TextFormat("FPS: %i", GetFPS()), 16, 16, 32, GREEN);
      DrawText(TextFormat("Player Position: (%02.02f, %02.02f)",
                          player->position.x, player->position.y), 
               16, 48, 32, GREEN);
    }
  }
  EndDrawing();
}
