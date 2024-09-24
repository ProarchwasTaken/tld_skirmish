// utils_enemies.h
#pragma once
#include "base/generics.h"


/* Consists of functions specifically for the game's enemies. Including
 * but not limited to: functions for manipulating a list of enemies.*/
namespace Enemies {
  /* Does exactly what is says on the tin. Automatically deletes enemies
   * within that are awaiting deletion, and creates a new list with the
   * deleted enemy excluded. Should be called once every frame.*/
  void deleteDeadEnemies(combatant_list &enemies);

  /* This function is meant to be used for debug purposes. It draws an
   * enemy's patience value some distance above their bounding box.*/
  void drawPatience(Combatant *enemy, uint8_t patience, Color color, 
                    int y_offset);
}

