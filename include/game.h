// game.h
#pragma once
#include <memory>
#include <raylib.h>
#include "sys_sprites.h"
#include "sys_audio.h"
#include "sys_transition.h"
#include "base/scene.h"


/* As you may have guessed, this class is important as hell as it defines
 * the structure of the game itself. Holds several important properties
 * such as the game's state, the canvas of where everything will be drawn,
 * and pointers to other important systems, and game objects.*/
class Game {
public:
  Game(bool debug_scene);
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

  /* A function template for loading a specific scene into memory, while
   * also clearing the currently loaded scene beforehand. This holds
   * the potential of each scene having their distinct parameters*/
  template<class SceneType, typename... Args>
  void loadScene(Args&&... args) {
    if (scene != nullptr) {
      scene.reset();
    }

    scene = std::make_unique<SceneType>(*this, args...); 
  }

  /* The root function for checking for inputs, updating all active game
   * elements, and drawing the screen. All based on the game's current
   * gamestate.*/
  void refresh();

  WipeTransition transition;
private:
  RenderTexture canvas;
  Rectangle canvas_source;
  Rectangle canvas_dest;

  Texture bg_main;

  Font skirmish_font;

  std::unique_ptr<SpriteLoader> sprite_loader;
  std::unique_ptr<AudioManager> audio_manager;
  std::unique_ptr<Scene> scene;
};
