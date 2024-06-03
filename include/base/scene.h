// scene.h
#pragma once


class Scene {
public:
  Scene();
  ~Scene();

  virtual void checkInput() {};
  virtual void updateScene(double &delta_time) {};
  virtual void drawScene() {};
};
