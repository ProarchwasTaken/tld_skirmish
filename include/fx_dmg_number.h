// fx_dmg_number.h
#pragma once
#include <raylib.h>
#include "base/dynamic_actor.h"


/* DamageNumber is a DynamicActor that's of an special effect more than
 * anything. Not meant to affect anything in the game physically. It's
 * a number that's floats up for a bit before disappearing. They're
 * typically used to indicate how much damage an attack has inflicted.*/
class DamageNumber : public DynamicActor {
public:
  DamageNumber(int value, Vector2 position, Color color);
  ~DamageNumber() override;

  void update() override;
  void movement();

  void draw(Vector2 &camera_target) override;
private:
  Texture number_texture;
  Color tint = WHITE;

  float movement_speed;
  float distance_traveled = 0;
};
