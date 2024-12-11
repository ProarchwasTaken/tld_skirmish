// game.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "sys_sprites.h"
#include "sys_audio.h"
#include "utils_music.h"
#include "scene_splash.h"
#if DEV_BUILD
#include "scene_debug.h"
#endif // DEV_BUILD
#include <plog/Log.h>

using std::make_unique;


Game::Game(bool debug_scene) {
  setupCanvas();
  defineColorPalette();
  loadGameFonts();

  bg_main = LoadTexture("graphics/bg_main.png");
  sprite_loader = make_unique<SpriteLoader>();
  audio_manager = make_unique<AudioManager>();

#if DEV_BUILD
  if (debug_scene) {
    loadScene<DebugScene>();
    return;
  }
#endif // DEV_BUILD
  loadScene<SplashScene>();
}

Game::~Game() {
  PLOGI << "Request to terminate program detected.";
  UnloadRenderTexture(canvas);
  UnloadImagePalette(COLORS::PALETTE);
  UnloadImage(GAME_ICON);

  UnloadTexture(bg_main);
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

void Game::refresh() {
  if (transition.active == false) {
    scene->checkInput();
  }

  transition.interpolate();
  MusicUtils::interpolateVolume();
  scene->updateScene();

  BeginTextureMode(canvas);
  {
    ClearBackground(BLACK);
    scene->drawScene();

    if (DEBUG_MODE) scene->drawDebugInfo();
    transition.draw();
  }
  EndTextureMode();

  BeginDrawing();
  {
    DrawTexturePro(canvas.texture, canvas_source, canvas_dest, 
                   {0, 0}, 0, WHITE);
  }
  EndDrawing();
}
