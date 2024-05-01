#include <raylib.h>
#include "defaults.h"


void resizeWindow(Rectangle &canvas) {
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


int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
             "True Human Tribulation: Skirmish");
  SetTargetFPS(TARGET_FRAMERATE);

  RenderTexture canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
  Rectangle source = {0, 0, CANVAS_WIDTH, -CANVAS_HEIGHT};
  Rectangle dest = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

  Texture test_texture = LoadTexture("res_test.png");

  while (WindowShouldClose() == false) {
    if (IsWindowResized()) {
      resizeWindow(dest);
    }
    if (IsKeyPressed(KEY_F11)) {
      ToggleBorderlessWindowed();
      resizeWindow(dest);
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
  return 0;
}
