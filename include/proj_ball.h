// proj_ball.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/generics.h"
#include "base/dynamic_actor.h"


class BallProjectile : public DynamicActor {
public:
  BallProjectile(Vector2 position, PlayerCharacter *player, 
                 combatant_list *enemies);

  void update() override;
  void draw(Vector2 &camera_target) override;
private:
  PlayerCharacter *player;
  combatant_list *enemies;

  int8_t direction;

  std::vector<int> anim_spin;
  float spin_frametime;
};
