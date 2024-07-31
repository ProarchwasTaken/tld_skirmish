// proj_knife.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/generics.h"
#include "base/dynamic_actor.h"


class KnifeProjectile : public DynamicActor {
public:
  KnifeProjectile(Vector2 position, int8_t direction, 
                  combatant_list *enemies);

  void update(double &delta_time) override;
  void draw() override;
private:
  combatant_list *enemies;

  int8_t direction;
  uint16_t damage;

  float stun_time;

  std::vector<int> anim_spin;
  float spin_frametime;
};
