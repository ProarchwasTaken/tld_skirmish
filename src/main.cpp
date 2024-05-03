#include <raylib.h>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "defaults.h"
#include "game.h"
#include "color_palette.h"

using plog::RollingFileAppender, plog::TxtFormatter, 
plog::ColorConsoleAppender;

Color *COLORS::PALETTE;


void setupCustomLogger() {
  static RollingFileAppender<TxtFormatter> file_appender("logs/log.txt",
                                                           1000000, 10);
  static ColorConsoleAppender<TxtFormatter> console_appender;
  plog::init(plog::verbose, &file_appender)
    .addAppender(&console_appender);
  PLOGV << "Logger initialized.";
}


int main() {
  setupCustomLogger();

  PLOGI << "Initializing the game...";
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
             "True Human Tribulation: Skirmish");
  SetTargetFPS(TARGET_FRAMERATE);

  Game game;

  PLOGV << "Everything seems good to go!";
  while (WindowShouldClose() == false) {
    if (IsWindowResized()) {
      game.correctWindow();
    }
    if (IsKeyPressed(KEY_F11)) {
      ToggleBorderlessWindowed();
      game.correctWindow();
    }

    game.refresh();
  }

  return 0;
}
