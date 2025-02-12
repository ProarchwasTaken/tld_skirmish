// game.cpp
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <string>
#include <filesystem>
#include <chrono>
#include <triblib/sys_audio.h>
#include "defaults.h"
#include "globals.h"
#include "game.h"
#include "system/sprites.h"
#include <triblib/utils/music.h>
#include "utils/settings.h"
#include "scenes/scene_splash.h"
#if DEV_BUILD
#include "scenes/scene_debug.h"
#endif // DEV_BUILD
#include <plog/Log.h>

using std::make_unique, std::filesystem::create_directory, std::string,
std::chrono::system_clock;


Game::Game(bool debug_scene) {
  setupCanvas();
  defineColorPalette();
  loadGameFonts();
  fullscreenCheck();

  bg_main = LoadTexture("graphics/bg_main.png");
  sprite_loader = make_unique<SpriteLoader>();
  audio_manager = make_unique<AudioManager>("audio/audio_data.toml");

#if DEV_BUILD
  if (debug_scene) {
    loadScene<DebugScene>();
    return;
  }
#else
  SetExitKey(KEY_NULL);
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

  Settings::save();
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

void Game::takeScreenshot() {
  if (DirectoryExists("screenshots") == false) {
    PLOGD << "'screenshots' directory not found!";
    create_directory("screenshots");
  }

  system_clock::time_point today = system_clock::now();
  long time = system_clock::to_time_t(today);

  string file_path = "screenshots/skirmish_" + 
    std::to_string(time) + ".png";

  Image screenshot = LoadImageFromScreen();
  ExportImage(screenshot, file_path.c_str());
}

void Game::fullscreenCheck() {
  if (IsWindowState(FLAG_WINDOW_UNDECORATED) != settings::fullscreen) {
    ToggleBorderlessWindowed();
    correctWindow();
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
