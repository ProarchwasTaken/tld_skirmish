// utils/text.cpp
#include <cmath>
#include <raylib.h>
#include <string>
#include "utils.h"


Vector2 Text::alignCenter(Font *font, std::string text, Vector2 position, 
                          int size_multiplier, int spacing)
{
  int font_size = font->baseSize * size_multiplier;
  int txt_width = MeasureTextEx(*font, text.c_str(), 
                                font_size, spacing).x;

  position.x -= round(txt_width / 2.0);
  return position;
}

Vector2 Text::alignRight(Font *font, std::string text, Vector2 position, 
                         int size_multiplier, int spacing)
{
  int font_size = font->baseSize * size_multiplier;
  int txt_width = MeasureTextEx(*font, text.c_str(), 
                                font_size, spacing).x;

  position.x -= txt_width;
  return position;
}
