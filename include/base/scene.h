// scene.h
#pragma once


class Scene {
public:
  Scene() {};
  virtual ~Scene() {};

  virtual void checkInput() {};
  virtual void updateScene(double &delta_time) {};
  virtual void drawScene() {};
  virtual void drawDebugInfo() {};
};
