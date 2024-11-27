// scene/scene_pregame.cpp
#include <array>
#include <string>
#include <cstdint>
#include <random>
#include "base/scene.h"
#include "globals.h"
#include "game.h"
#include "utils_text.h"
#include "scene_pregame.h"
#include "scene_gameplay.h"
#include <plog/Log.h>
#include <raylib.h>

using std::array, std::string, std::uniform_int_distribution;

array<string, 2> message_pool = {
  "Resolve allows you to last a little bit longer.",
  "The will could never surpass the flesh."
};


PregameScene::PregameScene(Game &skirmish, uint8_t weapon_id): 
  Scene(skirmish)
{
  this->weapon_id = weapon_id;
  start_timestamp = CURRENT_TIME;
  PLOGI << "Loaded Pregame scene.";
}

void PregameScene::updateScene() {
  float time_elapsed = CURRENT_TIME - start_timestamp;

  switch (phase) {
    case PHASE_BLANK1: {
      if (time_elapsed >= 0.5) {
        phase = PHASE_MSG1;

        setupFirstMsg();
        PLOGI << "Displayed first message.";
      }
      break;
    }
    case PHASE_MSG1: {
      if (time_elapsed >= 5.5) {
        phase = PHASE_MSG2;

        setupSecondMsg();
        PLOGI << "Displayed second message";
      }
      break;
    }
    case PHASE_MSG2: {
      if (time_elapsed >= 7.5) {
        phase = PHASE_BLANK2;

        message = "";
        PLOGI << "Cleared message.";
      }
      break;
    }
  }

  if (time_elapsed >= start_time) {
    skirmish->loadScene<GameplayScene>(weapon_id);
  }
}

void PregameScene::setupFirstMsg() {
  message = getRandomMsg();

  msg_position = Text::alignCenter(fonts::skirmish, message.c_str(), 
                                   {213, 104}, 1, spacing);
}

std::string PregameScene::getRandomMsg() {
  PLOGD << "Attempting to get a random message.";
  int end_range = message_pool.size() - 1;
  assert(end_range > 0);

  if (end_range == 0) {
    PLOGD << "Detected only one message in message pool.";
    return message_pool[0];
  }
  else {
    auto range = uniform_int_distribution<int>(0, end_range);
    int index = range(RNG::generator);

    PLOGD << "Using message at index: " << index;
    return message_pool[index];
  }
}

void PregameScene::setupSecondMsg() {
  message = "Survive until daylight.";

  msg_position = Text::alignCenter(fonts::skirmish, message.c_str(), 
                                   {213, 104}, 1, spacing);
}

void PregameScene::drawScene() {
  int size = fonts::skirmish->baseSize;
  DrawTextEx(*fonts::skirmish, message.c_str(), msg_position, size, 
             spacing, WHITE);
}
