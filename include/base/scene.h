// scene.h
#pragma once
#include <functional>


/* A scene is more like an collection of game elements. A enclosed space
 * of which game objects of initialized, and it's practically the third
 * and lowest level of the program. Think of it as how scenes are normally
 * done in game engines like Unity. (Even though I despise that 
 * comparison.)*/
class Scene {
public:
  Scene(std::function<void(int)> load_scene) {
    this->load_scene = load_scene;
  };
  virtual ~Scene() {};

  std::function<void(int)> load_scene;

  virtual void checkInput() {};
  virtual void updateScene(double &delta_time) {};
  virtual void drawScene() {};
  virtual void drawDebugInfo() {};
};
