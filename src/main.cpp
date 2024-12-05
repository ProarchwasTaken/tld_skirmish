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
bool parseArguments(int argc, char *argv[]);


int main(int argc, char *argv[]) {
  setupCustomLogger();
  bool debug_scene = parseArguments(argc, argv);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "THT II: Skirmish - v" VERSION);
  SetTargetFPS(TARGET_FRAMERATE);

  InitAudioDevice();
  SetTextLineSpacing(11);
  GAME_ICON = LoadImage("graphics/game_icon.png");
  SetWindowIcon(GAME_ICON);

  Game skirmish(debug_scene);

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

    if (DEV_BUILD && IsKeyPressed(KEY_F3)) {
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


bool parseArguments(int argc, char *argv[]) {
  PLOGI << "Parsing command line arguments.";
  bool debug_scene = false;

  for (int x = 0; x < argc; x++) {
    string arg = argv[x];
    if (DEV_BUILD && arg == "--debug_scene") {
      PLOGI << "Starting the game in the debug scene.";
      debug_scene = true;
    }
    if (arg == "-v") {
      plog::get()->setMaxSeverity(plog::verbose); 
      PLOGV << "Has set logger severity to verbose.";
    }
  }

  return debug_scene;
}

