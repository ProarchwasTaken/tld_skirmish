// game.h
#pragma once
#include <memory>
#include <raylib.h>
#include "base/generics.h"
#include "char_player.h"

#define CAMERA_BOUNDS 512

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

  /* For defining the COLOR::PALETTE external variable to be properly 
   * used. This should only be called once, and only before the
   * color palette is actually used.*/
  void defineColorPalette();

  /* For keeping the player at the center of the screen at all times.
   * Unless the camera has hit a boundary. Called once every frame of
   * which the player is off-center.*/
  void cameraFollowPlayer();

  /* The root function for checking for inputs, updating all active game
   * elements, and drawing the screen. All based on the game's current
   * gamestate.*/
  void refresh();
private:
  RenderTexture canvas;
  Rectangle canvas_source;
  Rectangle canvas_dest;

  double delta_time;

  Texture test_room;
  Camera2D camera;

  combatant_list enemies;
  std::shared_ptr<PlayerCharacter> player;
};
