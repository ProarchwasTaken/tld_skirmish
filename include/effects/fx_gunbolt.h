// fx_gunbolt.h
#pragma once
#include <vector>
#include <triblib/base/dynamic_actor.h>
#include "combatants/char_player.h"


class GunBolt : public DynamicActor {
public:
  GunBolt(PlayerCharacter *player, float length);

  void update() override;
  void draw(Vector2 &camera_target) override;
private:
  PlayerCharacter *player;

  std::vector<int> anim_bolt = {1, 2, 3};
  float bolt_frametime = 0.05;
};
