// scene_gameplay.h
#pragma once
#include <memory>
#include <functional>
#include <raylib.h>
#include <cstdint>
#include "base/generics.h"
#include "base/scene.h"
#include "hud_life.h"
#include "char_player.h"


/* The most vital scene as it's where everything related to the main
 * gameplay loop will take place. (This is supposed to be a game btw.)
 * There are many important components that make up this scene.*/
class GameplayScene : public Scene {
public:
  GameplayScene(std::function<void(int)> load_scene);
  ~GameplayScene() override;

  void drawTimer();
  void drawWaveCount();

  void checkInput() override;
  void updateScene(double &delta_time) override;
  void drawScene() override;
private:
  Texture background;
  Texture overlay;

  Camera2D camera;

  uint8_t max_wave;
  uint8_t wave = 0;

  uint16_t timer;
  float tick_interval;
  float tick_timestamp = 0;

  std::shared_ptr<PlayerCharacter> player;
  std::unique_ptr<LifeHud> life_hud;
  combatant_list enemies;
};
