// scene_gameplay.h
#pragma once
#include <memory>
#include <functional>
#include <raylib.h>
#include "base/generics.h"
#include "base/scene.h"
#include "hud_life.h"
#include "char_player.h"


class GameplayScene : public Scene {
public:
  GameplayScene(std::function<void(int)> load_scene);
  ~GameplayScene() override;

  void checkInput() override;
  void updateScene(double &delta_time) override;
  void drawScene() override;
private:
  Texture background;
  Texture overlay;

  Camera2D camera;

  std::shared_ptr<PlayerCharacter> player;
  std::unique_ptr<LifeHud> life_hud;
  combatant_list enemies;
};
