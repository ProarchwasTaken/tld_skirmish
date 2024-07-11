// utils/menu.cpp
#include "base/generics.h"
#include "utils.h"


void Menu::nextOption(menu_options &options, 
                      menu_options::iterator &selected, bool loop)
{
  selected++;
  
  if (selected != options.end()) {
    return;
  }

  if (loop == true) {
    selected = options.begin();
  }
  else {
    selected--;
  }
}

void Menu::previousOption(menu_options &options, 
                          menu_options::iterator &selected, bool loop)
{
  if (selected == options.begin() && loop == true) {
    selected = options.end();
    selected--;
  }
  else if (selected != options.begin()) {
    selected--; 
  }

}
