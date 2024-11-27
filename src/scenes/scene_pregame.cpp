// scene/scene_pregame.cpp
#include <cstdint>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "scene_pregame.h"
#include "scene_gameplay.h"
#include <plog/Log.h>


PregameScene::PregameScene(Game &skirmish, uint8_t weapon_id): 
  Scene(skirmish)
{
  this->weapon_id = weapon_id;
  start_timestamp = CURRENT_TIME;
  PLOGI << "Loading Pregame scene.";
}

void PregameScene::updateScene() {
  float time_elapsed = CURRENT_TIME - start_timestamp;

  if (time_elapsed >= start_time) {
    skirmish->loadScene<GameplayScene>(weapon_id);
  }
}
