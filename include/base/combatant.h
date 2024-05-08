// combatant.h
#pragma once
#include <cstdint>
#include <string>
#include <raylib.h>
#include "actor.h"

#define CBT_PLAYER 0
#define CBT_ENEMY 1

#define NEUTRAL 0
#define CHARGING 1
#define ACT 2
#define RECOVER 3
#define HIT_STUN 4

class Combatant : public Actor {
public:
  Combatant(std::string name, uint8_t type, uint16_t max_health,
            Vector2 position, Vector2 hitbox_scale = {32, 64},
            Vector2 tex_scale = {64, 64});

  std::string name;
  uint16_t max_health;
  uint8_t type;

  uint16_t health;
  uint8_t state;

  float charge_time;
  float act_time;
  float recovery_time;
  float stun_time;
};
