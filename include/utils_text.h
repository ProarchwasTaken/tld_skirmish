// utils_text.h
#pragma once
#include <raylib.h>
#include <string>


/* Helper functions for manipulating text, and their positions in a
 * certain way. Like aligning them to the right or center.*/
namespace Text {
  Vector2 alignCenter(Font *font, std::string text, Vector2 position, 
                      int size_multiplier, int spacing);

  Vector2 alignRight(Font *font, std::string text, Vector2 position,
                     int size_multiplier, int spacing);
}

