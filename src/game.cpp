// game.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "sprite_loader.h"
#include "audio.h"
#include "scene_debug.h"
#include <plog/Log.h>

using std::make_unique;


Game::Game(int start_scene) {
  setupCanvas();
  defineColorPalette();

  sprite_loader = make_unique<SpriteLoader>();
  sprite_loader->loadSpritesheet({"player", "ghoul"});

  audio_manager = make_unique<AudioManager>();

  loadScene(start_scene);
}

Game::~Game() {
  PLOGV << "Request to terminate program detected.";
  UnloadRenderTexture(canvas);
  UnloadImagePalette(COLORS::PALETTE);

  scene.reset();
  sprite_loader.reset();
  audio_manager.reset();

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

void Game::loadScene(int scene_id) {
  if (scene != nullptr) {
    scene.reset();
  }

  PLOGI << "Attempting to load scene correlated with id: " << scene_id;
  switch (scene_id) {
    case SCENE_DEBUG: {
      scene = make_unique<DebugScene>();
      break;
    }
    case SCENE_MENU: 
    case SCENE_GAMEPLAY: {
      PLOGE << "Scene is not implemented yet!";
      throw SCENE_NOT_IMPLEMENTED;
    }
    default: {
      PLOGE << "The scene correlated with ID: " << scene_id << " doesn't"
        " exist!";
      throw SCENE_NOT_FOUND;
    }
  }
}

void Game::refresh() {
  delta_time = GetFrameTime() * TARGET_FRAMERATE;

  scene->checkInput();
  scene->updateScene(delta_time);

  BeginTextureMode(canvas);
  {
    ClearBackground(BLACK);
    scene->drawScene();
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);

    if (DEBUG_MODE) {
      scene->drawDebugInfo();
    }
  }
  EndDrawing();
}
