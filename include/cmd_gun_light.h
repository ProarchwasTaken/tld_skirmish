// cmd_gun_light.h
#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include "base/action_command.h"
#include "base/combatant.h"
#include "base/generics.h"
#include "char_player.h"


class GunLight : public ActionCommand {
public:
  GunLight(PlayerCharacter *player);
  ~GunLight();

  void chargeSequence(float time_elapsed) override;
  void enemyHitCheck();

  Rectangle setupHurtbox();
  void enemyDetection(std::vector<Combatant*> *detected_enemies);
  void probeClosestEnemy(std::vector<Combatant*> &detected_enemies);

  void actSequence(float time_elapsed) override;

  bool techInputHeldDown();

  void wrapup();
  void detachProbes();

  void slowMovement();
  void tickDamage();
  void moraleDrain();
  void rangeCheck();

  bool hit_enemy = false;
private:
  combatant_list *enemies;
  PlayerCharacter *player;

  float max_range = 128;
  Combatant *probed_enemy = NULL;

  float tick_time = 0.75;
  float tick_timestamp = 0;
  uint8_t tick_damage = 2; 

  float drain_time = 0.35;
  float drain_timestamp = 0;
  uint8_t drain_magnitude = 2;

  std::vector<int> anim_slow = {29, 30, 31, 30};
  float slow_frametime = 0.15;
};
