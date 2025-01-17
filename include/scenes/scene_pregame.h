// scene_pregame.h
#pragma once
#include <cstdint>
#include <string>
#include <triblib/base/scene.h>
#include "game.h"

#define PHASE_BLANK1 0
#define PHASE_MSG1 1
#define PHASE_MSG2 2
#define PHASE_BLANK2 3

class PregameScene : public Scene {
public:
  PregameScene(Game &skirmish, uint8_t weapon_id);

  void updateScene() override;
  void setupFirstMsg();
  std::string getRandomMsg();

  void setupSecondMsg();

  void drawScene() override;
private:
  uint8_t weapon_id;

  float start_time = 8;
  float start_timestamp;

  uint8_t phase = PHASE_BLANK1;

  std::string message = "";
  Vector2 msg_position = {213, 104};
  int spacing = -2;
};
