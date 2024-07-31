// utils/dynamic.cpp
#include <memory>
#include <type_traits>
#include <vector>
#include <algorithm>
#include "base/generics.h"
#include "globals.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include <plog/Log.h>

using std::vector, std::is_base_of, std::make_unique, std::unique_ptr;

/* Comparison function used for sorting a list from least to greatest
 * according to the DynamicActor's type.*/
bool typeCompare(unique_ptr<DynamicActor> &d1, 
                 unique_ptr<DynamicActor> &d2);

/* Comparison function that sorts the list a way that the Dynamic Actors 
 * that are awaiting deletion will end up at the front of the list. Useful
 * for when trying to erase multiple DActors at once.*/
bool deleteCompare(unique_ptr<DynamicActor> &d1, 
                   unique_ptr<DynamicActor> &d2);


template<class DerivedClass, typename... Args>
void Dynamic::create(Args&&... args) {
  if (is_base_of<DynamicActor, DerivedClass>::value == false) {
    PLOGE << "DerivedClass does not derive from Base!";
    throw;
  }

  PLOGI << "Creating DynamicActor: DerivedClass...";
  queue.push_back(make_unique<DerivedClass>(args...));
  PLOGD << "Added DerivedClass to the queue.";
}


void Dynamic::moveFromQueue(dynamic_list &main_list) {
  if (queue.size() == 0) {
    return;
  }

  PLOGI << "Moving Dynamic Actors from queue to main list.";
  std::move(queue.begin(), queue.end(), main_list.end());

  PLOGI << "Now sorting main list.";
  std::sort(main_list.begin(), main_list.end(), typeCompare);
}

void Dynamic::clearAwaitingDeletion(dynamic_list &main_list) {
  int count = main_list.size();
  if (count == 0) {
    return;
  }

  int deleted;
  for (int index = 0; index < count; index++) {
    auto &dynamic_actor = main_list[index];

    if (dynamic_actor->awaiting_deletion) {
      dynamic_actor.reset();
      deleted++;
    }
  }
  PLOGI << "Deleted: " << deleted << "Dynamic Actors";

  if (deleted != 0) {
    std::sort(main_list.begin(), main_list.end(), deleteCompare);

    main_list.erase(main_list.begin(), main_list.begin() + deleted);

    std::sort(main_list.begin(), main_list.end(), typeCompare);
  }
}


bool typeCompare(unique_ptr<DynamicActor> &d1, 
                 unique_ptr<DynamicActor> &d2)
{
  return d1->type < d2->type;
}


bool deleteCompare(unique_ptr<DynamicActor> &d1, 
                   unique_ptr<DynamicActor> &d2)
{
  return d1->awaiting_deletion > d2->awaiting_deletion;
}

