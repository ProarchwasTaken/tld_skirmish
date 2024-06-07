// enemy_ghoul.h
#include <raylib.h>
#include "base/combatant.h"
#include "char_player.h"

#define GOL_HP 50
#define GOL_HITBOX_SCALE (Vector2){24, 56} 
#define GOL_HITBOX_OFFSET (Vector2){-12, -58}


class GhoulEnemy : public Combatant {
public:
  GhoulEnemy(PlayerCharacter &player, Vector2 position);

  void update(double &delta_time) override;
  void draw() override;
private:
  PlayerCharacter *player;
};
