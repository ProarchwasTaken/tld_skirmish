// main.cpp
#include <raylib.h>
#include <string>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Severity.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "defaults.h"
#include "game.h"
#include "globals.h"

using plog::RollingFileAppender, plog::TxtFormatter, std::string, 
plog::ColorConsoleAppender;

void setupCustomLogger();
int parseArguments(int argc, char *argv[]);


int main(int argc, char *argv[]) {
  setupCustomLogger();
  int start_scene = parseArguments(argc, argv);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "THT II: Skirmish - v" VERSION);
  SetTargetFPS(TARGET_FRAMERATE);
  InitAudioDevice();

  Game skirmish(start_scene);

  PLOGI << "Everything seems good to go!";
  PLOGI << "Startup Time: " << GetTime();
  while (EXIT_GAME == false) {
    EXIT_GAME = WindowShouldClose();
    if (IsWindowResized()) {
      skirmish.correctWindow();
    }

    if (IsKeyPressed(KEY_F11)) {
      PLOGI << "Toggling borderless fullscreen.";
      ToggleBorderlessWindowed();
      skirmish.correctWindow();
    }

    if (IsKeyPressed(KEY_F3)) {
      PLOGI << "Toggling Debug Mode";
      DEBUG_MODE = !DEBUG_MODE;
    }

    skirmish.refresh();
  }

  CloseWindow();
  return 0;
}


void setupCustomLogger() {
  static RollingFileAppender<TxtFormatter> file_appender("logs/log.txt",
                                                           1000000, 10);
  static ColorConsoleAppender<TxtFormatter> console_appender;
  plog::init(plog::info, &file_appender)
    .addAppender(&console_appender);

  PLOGI << "Skirmish " << VERSION << " - " << VER_STAGE << " Build";
  PLOGI << "Open Source project originally made by: " << AUTHOR;
  PLOGI << "Github Repository: " << HOME_URL;
}


int parseArguments(int argc, char *argv[]) {
  PLOGI << "Parsing command line arguments.";
  int start_scene = SCENE_TITLE;

  for (int x = 0; x < argc; x++) {
    string arg = argv[x];
    if (DEV_BUILD && arg == "--debug_scene") {
      PLOGI << "Starting the game in the debug scene.";
      start_scene = SCENE_DEBUG;
    }
    if (arg == "-v") {
      plog::get()->setMaxSeverity(plog::verbose); 
      PLOGV << "Has set logger severity to verbose.";
    }
  }

  return start_scene;
}

