// game.cpp
#include <raylib.h>
#include <memory>
#include "defaults.h"
#include "game.h"
#include "color_palette.h"
#include "char_player.h"
#include <plog/Log.h>

using std::make_unique;


Game::Game() {
  setupCanvas();
  defineColorPalette();

  test_room = LoadTexture("concept_art/test_room1.png");
  player = make_unique<PlayerCharacter>();
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

void Game::refresh() {
  delta_time = GetFrameTime() * TARGET_FRAMERATE;
  player->inputPressed();
  player->inputReleased();

  player->update(delta_time);

  BeginTextureMode(canvas);
  {
    ClearBackground(BLACK);
    DrawTexture(test_room, 0, 0, WHITE);
    player->draw();
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);
  }
  EndDrawing();
}
