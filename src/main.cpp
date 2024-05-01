#include <raylib.h>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "defaults.h"

using plog::RollingFileAppender, plog::TxtFormatter, 
plog::ColorConsoleAppender;


/* For correcting the dimensions of the canvas to the correct window
 * resolution. Sometimes the window my able change height just to 
 * fit perfectly with the canvas' aspect ratio.*/
void correctWindow(Rectangle &canvas) {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  double canvas_ratio = canvas.height / canvas.width;

  canvas.width = screen_width;

  if (IsWindowMaximized()) {
    canvas.height = screen_height;
  }
  else { 
    double calulated_height = canvas.width * canvas_ratio;
    canvas.height = calulated_height;
  }

  if (canvas.height < screen_height || canvas.height > screen_height){
    SetWindowSize(screen_width, canvas.height);
  }
}


void setupCustomLogger() {
  static RollingFileAppender<TxtFormatter> file_appender("logs/log.txt",
                                                           1000000, 10);
    static ColorConsoleAppender<TxtFormatter> console_appender;
    plog::init(plog::verbose, &file_appender)
      .addAppender(&console_appender);
}


int main() {
  setupCustomLogger();
  PLOGV << "Logger initialized.";
  PLOGI << "Initizing game, and setting everything up...";

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
             "True Human Tribulation: Skirmish");
  SetTargetFPS(TARGET_FRAMERATE);

  PLOGI << "Setting up canvas...";
  RenderTexture canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
  Rectangle source = {0, 0, CANVAS_WIDTH, -CANVAS_HEIGHT};
  Rectangle dest = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

  Texture test_texture = LoadTexture("res_test.png");

  PLOGV << "Everything seems good to go!";
  while (WindowShouldClose() == false) {
    if (IsWindowResized()) {
      correctWindow(dest);
    }
    if (IsKeyPressed(KEY_F11)) {
      ToggleBorderlessWindowed();
      correctWindow(dest);
    }

    BeginTextureMode(canvas);
    {
      ClearBackground(BLACK);
      DrawTexture(test_texture, 0, 0, WHITE);
    }
    EndTextureMode();

    BeginDrawing();
    {
      ClearBackground(BLACK);
      DrawTexturePro(canvas.texture, source, dest, {0, 0}, 0, WHITE);
    }
    EndDrawing();
  }

  UnloadRenderTexture(canvas);
  UnloadTexture(test_texture);
  PLOGV << "Thanks for playing!";
  return 0;
}
