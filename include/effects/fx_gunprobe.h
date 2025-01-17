// fx_gunprobe.h
#pragma once
#include <triblib/base/dynamic_actor.h>
#include "base/combatant.h"
#include "combatants/char_player.h"


class GunProbe : public DynamicActor {
public:
  GunProbe(PlayerCharacter *player, Combatant *probed_enemy);

  void update() override;

  bool shouldDelete();
  void positionCorrection();

  void draw(Vector2 &camera_target) override;
  
private:
  float y_level = 102;

  PlayerCharacter *player;
  Combatant *probed_enemy;
};
