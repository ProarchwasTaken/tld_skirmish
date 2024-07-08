// game.h
#pragma once
#include <memory>
#include <raylib.h>
#include "sprite_loader.h"
#include "audio.h"
#include "base/scene.h"

#define SCENE_STARTUP 0
#define SCENE_TITLE 1
#define SCENE_MAINMENU 2
#define SCENE_SETTINGS 3
#define SCENE_CONTROLS 4
#define SCENE_PREGAME 5

#define SCENE_GAMEPLAY 6
#define SCENE_DEBUG 7


#define SCENE_NOT_FOUND 4205
#define SCENE_NOT_IMPLEMENTED 9537

/* As you may have guessed, this class is important as hell as it defines
 * the structure of the game itself. Holds several important properties
 * such as the game's state, the canvas of where everything will be drawn,
 * and pointers to other important systems, and game objects.*/
class Game {
public:
  Game(int start_scene);
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

  /* What this method does is an absolute mystery, and it could not be
   * discern by just reading it's name.*/
  void loadGameFonts();

  /* Loads a specific scene into memory. A integer is used to specify what
   * scene will be loaded and every scene is identified by a number.
   * Throws an error when the scene the function tries to load doesn't
   * exist, or it hasn't been implemented yet.*/
  void loadScene(int scene_id);

  /* The root function for checking for inputs, updating all active game
   * elements, and drawing the screen. All based on the game's current
   * gamestate.*/
  void refresh();
private:
  RenderTexture canvas;
  Rectangle canvas_source;
  Rectangle canvas_dest;

  Font skirmish_font;

  std::unique_ptr<SpriteLoader> sprite_loader;
  std::unique_ptr<AudioManager> audio_manager;
  std::unique_ptr<Scene> scene;

  double delta_time;
};
