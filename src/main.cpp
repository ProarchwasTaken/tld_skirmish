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

using plog::RollingFileAppender, plog::TxtFormatter, std::string, 
plog::ColorConsoleAppender, std::vector;

Color *COLORS::PALETTE;
vector<Texture*> sprites::player; 
bool DEBUG_MODE = false;

void setupCustomLogger();
void parseArguments(int argc, char *argv[]);


int main(int argc, char *argv[]) {
  setupCustomLogger();
  if (PLATFORM == WINDOWS) {
    PLOGV << "Operating System: Windows";
  }
  else if (PLATFORM == LINUX) {
    PLOGV << "Operating System: Linux";
  }

  if (argc > 1) {
    parseArguments(argc, argv);
  }

  PLOGV << "Initializing the game...";
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
             "True Human Tribulation: Skirmish");
  SetTargetFPS(TARGET_FRAMERATE);

  int start_scene;
  if (DEBUG_MODE) {
    start_scene = SCENE_DEBUG;
  }
  else {
    start_scene = SCENE_MENU;
  }

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
}


void parseArguments(int argc, char *argv[]) {
  PLOGI << "Parsing command line arguments.";

  for (int x = 0; x < argc; x++) {
    string arg = argv[x];
    if (arg == "--debug") {
      PLOGV << "Starting the game in debug mode.";
      DEBUG_MODE = true;
    }
  }
}

