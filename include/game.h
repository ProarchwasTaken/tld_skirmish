// game.h
#pragma once
#include <raylib.h>


class Game {
public:
  Game();
  ~Game();

  /* For correcting the dimensions of the canvas to the correct window
   * resolution. Sometimes the window my able change height just to 
   * fit perfectly with the canvas' aspect ratio.*/
  void correctWindow();
  void setupCanvas();
  void refresh();
private:
  RenderTexture canvas;
  Rectangle canvas_source;
  Rectangle canvas_dest;
};
