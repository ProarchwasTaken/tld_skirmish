// utils/enemies.cpp
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "globals.h"
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

      PLOGD << "Appending enemy index: " << index << 
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

void Enemies::drawPatience(Combatant *enemy, uint8_t patience, 
                           Color color, int y_offset)
{
  Vector2 scale, position, offset;
  const char *text = TextFormat("%02i", patience);

  int size = fonts::skirmish->baseSize;

  scale = MeasureTextEx(*fonts::skirmish, text, size, -3);

  offset.x = -scale.x / 2;
  offset.y = enemy->tex_offset.y - scale.y - y_offset;
  position = Vector2Add(enemy->position, offset);

  DrawTextEx(*fonts::skirmish, text, position, size, -3, color);
}
