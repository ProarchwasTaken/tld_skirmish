// game.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <functional>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "sprite_loader.h"
#include "audio.h"
#include "scene_debug.h"
#include "scene_title.h"
#include "scene_menu.h"
#include "scene_gameplay.h"
#include <plog/Log.h>

using std::make_unique, std::function;


Game::Game(int start_scene) {
  setupCanvas();
  defineColorPalette();
  loadGameFonts();

  sprite_loader = make_unique<SpriteLoader>();
  sprite_loader->loadSpritesheet({
    "hud_life", "hud_morale", "player", "ghoul", "weapon_knife"
  });

  audio_manager = make_unique<AudioManager>();

  loadScene(start_scene);
}

Game::~Game() {
  PLOGI << "Request to terminate program detected.";
  UnloadRenderTexture(canvas);
  UnloadImagePalette(COLORS::PALETTE);

  UnloadFont(skirmish_font);

  scene.reset();
  sprite_loader.reset();
  audio_manager.reset();

  PLOGI << "Thanks for playing!";
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

void Game::loadGameFonts() {
  skirmish_font = LoadFont("graphics/fonts/skirmish_font.png");
  fonts::skirmish = &skirmish_font;
}

void Game::loadScene(int scene_id) {
  if (scene != nullptr) {
    scene.reset();
  }

  function<void(int)> load_func = [this](int scene_id){
    this->loadScene(scene_id);
  };

  PLOGI << "Attempting to load scene correlated with id: " << scene_id;
  switch (scene_id) {
    case SCENE_DEBUG: {
      scene = make_unique<DebugScene>(load_func);
      break;
    }
    case SCENE_TITLE: {
      scene = make_unique<TitleScene>(load_func);
      break;
    }
    case SCENE_MENU: {
      scene = make_unique<MenuScene>(load_func);
      break;
    }
    case SCENE_GAMEPLAY: {
      scene = make_unique<GameplayScene>(load_func);
      break;
    }
    case SCENE_STARTUP: 
    case SCENE_SETTINGS:
    case SCENE_CONTROLS:
    case SCENE_PREGAME: {
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

    if (DEBUG_MODE) scene->drawDebugInfo();
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);
  }
  EndDrawing();
}
