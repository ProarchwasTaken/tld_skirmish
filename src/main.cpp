#include <raylib.h>


int main() {
  InitWindow(1280, 720, "True Human Tribulation: Skirmish");
  SetTargetFPS(60);

  while (WindowShouldClose() == false) {
    BeginDrawing();
    {
      ClearBackground(WHITE);
    }
    EndDrawing();
  }

  return 0;
}
