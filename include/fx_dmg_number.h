// fx_dmg_number.h
#pragma once
#include <raylib.h>
#include "base/dynamic_actor.h"


class DamageNumber : public DynamicActor {
public:
  DamageNumber(int value, Vector2 position, Color color);
  ~DamageNumber();

  void update() override;
  void movement();

  void draw(Vector2 &camera_target) override;
private:
  Texture number_texture;
  Color tint = WHITE;

  float movement_speed;
  float distance_traveled = 0;
};
