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
  void tickDamage();
private:
  combatant_list *enemies;
  PlayerCharacter *player;

  float max_range = 128;
  Combatant *probed_enemy = NULL;
  bool hit_enemy = false;

  float tick_time = 0.75;
  float tick_timestamp = 0;

  uint8_t tick_cost = 2;
  uint8_t tick_damage = 2;
  

  std::vector<int> anim_charge = {28, 29};
  float charge_frametime = 0.2;
};