// utils/enemies.cpp
#include <raylib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "base/generics.h"
#include "base/combatant.h"
#include "utils.h"
#include <plog/Log.h>

using std::vector, std::shared_ptr, std::find;


void Enemies::deleteDeadEnemies(combatant_list &enemies) {
  int enemy_count = enemies.size();
  if (enemy_count == 0) {
    return;
  }

  vector<int> deleted; 

  for (int index = 0; index < enemy_count; index++) {
    shared_ptr<Combatant> enemy = enemies[index];

    if (enemy->awaiting_deletion) {
      PLOGD << "Deleting enemy: " << enemy->name << " from memory.";
      enemy.reset();

      PLOGI << "Appending enemy index: " << index << 
        " to 'deleted' list.";
      deleted.push_back(index);
    }
  }

  if (deleted.size() == 0) {
    return;
  }

  PLOGI << "Transfering all enemies to new list with the deleted enemies" 
  " excluded.";
  combatant_list new_enemy_list;
  vector<int>::iterator search_result;

  for (int index = 0; index < enemy_count; index++) {
    search_result = find(deleted.begin(), deleted.end(), index);
    bool is_excluded = search_result != deleted.end();

    if (is_excluded == false) {
      new_enemy_list.push_back(enemies[index]);
    }
  }

  // I'm not exactly sure whether replacing the enemies list with the new
  // one is gonna cause a memory leak or not.
  enemies.clear();
  enemies = new_enemy_list;
}
