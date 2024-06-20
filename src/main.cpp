// main.cpp
#include <raylib.h>
#include <vector>
#include <string>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "defaults.h"
#include "game.h"
#include "globals.h"
#include "audio.h"

using plog::RollingFileAppender, plog::TxtFormatter, std::string, 
plog::ColorConsoleAppender, std::vector;

Color *COLORS::PALETTE;

vector<SoundMetaData> audio::sfx_metadata;

vector<Texture*> sprites::player; 
vector<Texture*> sprites::ghoul;

bool DEBUG_MODE = false;

void setupCustomLogger();
int parseArguments(int argc, char *argv[]);


int main(int argc, char *argv[]) {
  setupCustomLogger();
  int start_scene = parseArguments(argc, argv);

  PLOGV << "Initializing the game...";
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
             "True Human Tribulation: Skirmish");
  SetTargetFPS(TARGET_FRAMERATE);
  InitAudioDevice();

  Game game(start_scene);

  PLOGV << "Everything seems good to go!";
  while (WindowShouldClose() == false) {
    if (IsWindowResized()) {
      game.correctWindow();
    }

    if (IsKeyPressed(KEY_F11)) {
      PLOGI << "Toggling borderless fullscreen.";
      ToggleBorderlessWindowed();
      game.correctWindow();
    }

    if (IsKeyPressed(KEY_F3)) {
      PLOGI << "Toggling Debug Mode";
      DEBUG_MODE = !DEBUG_MODE;
    }

    game.refresh();
  }

  return 0;
}


void setupCustomLogger() {
  static RollingFileAppender<TxtFormatter> file_appender("logs/log.txt",
                                                           1000000, 10);
  static ColorConsoleAppender<TxtFormatter> console_appender;
  plog::init(plog::verbose, &file_appender)
    .addAppender(&console_appender);
  PLOGV << "Logger initialized.";

  if (PLATFORM == WINDOWS) {
    PLOGV << "Operating System: Windows";
  }
  else if (PLATFORM == LINUX) {
    PLOGV << "Operating System: Linux";
  }
}


int parseArguments(int argc, char *argv[]) {
  PLOGI << "Parsing command line arguments.";
  int start_scene = SCENE_MENU;

  for (int x = 0; x < argc; x++) {
    string arg = argv[x];
    if (arg == "--debug_scene") {
      PLOGV << "Starting the game in the debug scene.";
      start_scene = SCENE_DEBUG;
    }
  }

  return start_scene;
}

