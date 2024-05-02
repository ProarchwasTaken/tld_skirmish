// game.cpp
#include "defaults.h"
#include "game.h"
#include <raylib.h>
#include <plog/Log.h>


Game::Game() {

}

Game::~Game() {
  PLOGV << "Request to terminate program detected.";
  UnloadRenderTexture(canvas);
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
  PLOGI << "Setting up canvas...";
  canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
  canvas_source = {0, 0, CANVAS_WIDTH, -CANVAS_HEIGHT};
  canvas_dest = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
}

void Game::refresh() {
  BeginTextureMode(canvas);
  {
    ClearBackground(BLACK);
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);
  }
  EndDrawing();
}
