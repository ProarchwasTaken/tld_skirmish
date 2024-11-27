// scene_pregame.h
#pragma once
#include <cstdint>
#include "base/scene.h"
#include "game.h"


class PregameScene : public Scene {
public:
  PregameScene(Game &skirmish, uint8_t weapon_id);
  void updateScene() override;
private:
  uint8_t weapon_id;

  float start_time = 3;
  float start_timestamp;
};
