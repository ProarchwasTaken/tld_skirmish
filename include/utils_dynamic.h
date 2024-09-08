// utils_dynamic.h
#include <memory>
#include "globals.h"
#include "base/generics.h"


/* Consists of functions tailored around DynamicActors, and classes that
 * derive from them. Pretty much stuff that's pretty much required to 
 * make it all work.*/
namespace Dynamic { 
  /* For creating an class instance derivative of DynamicActor, and 
   * adding to the queue to be tranferred into the gameplay scene later.
   * This is my first ever template function, so issues might pop up.*/
  template<class DerivedClass, typename... Args>
  void create(Args&&... args) {
    queue.push_back(
      std::make_unique<DerivedClass>(args...)
    );
  }
  
  /* Tranfers all Dynamic Actors in the queue to the dynamic_list 
   * specified. Also makes sures that the list is sorted from least,
   * greatest according to each DynamicActor's type.*/
  void moveFromQueue(dynamic_list &main_list);

  /* For removing any Dynamic Actors that are waiting deletion from
   * memory. Also erase the pointer who owned the cleared memory from
   * the list as well. Similar to deleteDeadEnemies, but the way it's
   * done is very much different.*/
  void clearAwaitingDeletion(dynamic_list &main_list);
}

