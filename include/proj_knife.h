// proj_knife.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/generics.h"
#include "base/dynamic_actor.h"


/* KnifeProjectile is an DynamicActor that created upon the player
 * successfully performing the KnifeHeavy ActionCommand. It simply moves
 * in one direction, inflicting damage to the first enemy it comes in
 * contact with before deleting itself.*/
class KnifeProjectile : public DynamicActor {
public:
  KnifeProjectile(Vector2 position, int8_t direction,
                  combatant_list *enemies);

  void update() override;
  void draw(Vector2 &camera_target) override;

  void movement();
  void enemyHitCheck();
private:
  combatant_list *enemies;

  float velocity_x;
  int8_t direction;

  uint16_t damage;
  float stun_time;

  std::vector<int> anim_spin;
  float spin_frametime;
};
