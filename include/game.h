// game.h
#pragma once
#include <raylib.h>


/* As you may have guessed, this class is important as hell as it defines
 * the structure of the game itself. Holds several important properties
 * such as the game's state, the canvas of where everything will be drawn,
 * and pointers to other important systems, and game objects.*/
class Game {
public:
  Game();
  ~Game();

  /* For correcting the dimensions of the canvas to the correct window
   * resolution. Sometimes the window my able change height just to 
   * fit perfectly with the canvas' aspect ratio.*/
  void correctWindow();

  /* The canvas is where everything in the game will be drawn on. The
   * canvas is actually smaller than the window itself being at a 
   * resolution of (426x240). Just scaled up to fit the windows 
   * dimensions. Sometimes, the window itself will shape itself to fit 
   * the canvas' aspect ratio. Refer to the correctWindow function for
   * more info on that.*/
  void setupCanvas();

  void defineColorPalette();

  /* The root function for checking for inputs, updating all active game
   * elements, and drawing the screen. All based on the game's current
   * gamestate.*/
  void refresh();
private:
  RenderTexture canvas;
  Rectangle canvas_source;
  Rectangle canvas_dest;

  Texture test_room;
};
