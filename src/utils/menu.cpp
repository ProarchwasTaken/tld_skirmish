// utils/menu.cpp
#include <triblib/base/generics.h>
#include <triblib/utils/sound.h>
#include "utils_menu.h"


void Menu::nextOption(menu_options &options, 
                      menu_options::iterator &selected, bool loop)
{
  selected++;
  
  if (selected != options.end()) {
    SoundUtils::play("opt_change");
    return;
  }

  if (loop == true) {
    SoundUtils::play("opt_change");
    selected = options.begin();
  }
  else {
    SoundUtils::play("opt_error");
    selected--;
  }
}

void Menu::previousOption(menu_options &options, 
                          menu_options::iterator &selected, bool loop)
{
  if (selected == options.begin() && loop == true) {
    selected = options.end();
    selected--;

    SoundUtils::play("opt_change");
    return;
  }
  else if (selected != options.begin()) {
    selected--; 

    SoundUtils::play("opt_change");
    return;
  }

  SoundUtils::play("opt_error");
}
