// scene.h
#pragma once
#include <functional>


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
