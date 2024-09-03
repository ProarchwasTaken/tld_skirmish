// data/data_weapon.h
#pragma once
#include <cstdint>
#include <cstring>

struct TechData {
  char name[16];
  char description[34];
  uint8_t mp_cost;
};

struct SubWeaponData {
  SubWeaponData(TechData tech_light, TechData tech_heavy) {
    this->tech_light = tech_light;
    this->tech_heavy = tech_heavy;
  }

  TechData tech_light;
  TechData tech_heavy;
};
