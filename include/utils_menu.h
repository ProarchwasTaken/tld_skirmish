// utils_menu.h
#pragma once
#include "base/generics.h"


/* Helper functions for menu navigation. These functions can be very
 * versatile when used correctly. In fact, it's really funny how simple
 * these functions are. You should take a look.*/
namespace Menu {
  void nextOption(menu_options &options, 
                  menu_options::iterator &selected, bool loop);
  void previousOption(menu_options &options, 
                      menu_options::iterator &selected, bool loop);
}

