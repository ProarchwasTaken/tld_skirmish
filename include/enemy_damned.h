// enemy_damned.h
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/combatant.h"
#include "char_player.h"

#define DAM_HP 25
#define DAM_STABILITY 0.50

#define DAM_HITBOX_SCALE (Vector2){18, 56}
#define DAM_HITBOX_OFFSET (Vector2){-9, -58}


class DamnedEnemy : public Combatant {
public:
  DamnedEnemy(PlayerCharacter &player, Vector2 position);
  void update() override;

  void neutralBehavior();
  void stepForward();

  void draw(Vector2 &camera_target) override;
  void drawDebug() override;

  PlayerCharacter *player;
private:
  std::vector<int> anim_walk;
  float walk_frametime;

  std::vector<int> anim_death;
  float death_frametime;

  uint16_t player_dist;
  uint16_t preferred_dist;
  
  float step_distance;
};
