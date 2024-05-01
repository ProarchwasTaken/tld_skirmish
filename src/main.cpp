#include <raylib.h>
#include "constants.h"


int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
             "True Human Tribulation: Skirmish");
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
