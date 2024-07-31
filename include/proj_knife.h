// proj_knife.h
#pragma once
#include <raylib.h>
#include <vector>
#include "base/dynamic_actor.h"


class KnifeProjectile : public DynamicActor {
public:
  KnifeProjectile(Vector2 position);

  void update(double &delta_time) override;
private:
  std::vector<int> anim_spin;
  float spin_frametime;
};
