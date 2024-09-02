// scene.h
#pragma once
#include "base/generics.h"


/* A scene is more like an collection of game elements. A enclosed space
 * of which game objects of initialized, and it's practically the third
 * and lowest level of the program. Think of it as how scenes are normally
 * done in game engines like Unity. (Even though I despise that 
 * comparison.)*/
class Scene {
public:
  Scene(Game &skirmish) {
    this->skirmish = &skirmish;
  };
  virtual ~Scene() {};

  Game *skirmish;

  virtual void checkInput() {};
  virtual void updateScene() {};
  virtual void drawScene() {};
  virtual void drawDebugInfo() {};
};
