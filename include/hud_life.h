// hud_life.h
#pragma once
#include <raylib.h>
#include "char_player.h"


/* The LifeHud is a hud element that measures the player's health in two
 * forms. By text detailing the exact amount of health the player has 
 * left, and through a segmented life gauge that is much faster to read.
 * Pretty standard stuff really.*/
class LifeHud {
public:
  LifeHud(PlayerCharacter *player);
  void update();

  /* The color of certain element of the LifeHud will change depending
   * on the state of the player and the game. This function should always
   * be called once every frame.*/
  void determineHudColor();

  /* The life gauge measures the player's health in the form of 10 
   * segments. Each representing a percentage of the player's health in
   * multiples of 10.*/
  void updateGauge();

  /* If the percentage of the player's health is not a multiple of 10,
   * the last segment of the life gauge will "blink" to indicate that.*/
  void segmentBlinkInterval();

  /* Aligns the text so it would be anchored from the right rather than
   * the left. This will always to called every frame to account for
   * the possibility of the player's health changing.*/
  void alignText(const char* health_text);

  /* The life text is meant to display the exact amount of health the
   * player has left, relative to their maximum amount of health.*/
  void drawLifeText();
  void draw();

private:
  PlayerCharacter *player;
  float life_percentage;
  
  Vector2 hud_position;
  Color hud_color;
  Vector2 text_position;

  Vector2 gauge_position;
  Rectangle gauge_source; 

  float gauge_width;
  float segment_width;

  Rectangle segment_source;
  Vector2 segment_position;
  bool display_segment = false;

  float blink_interval;
  float blink_timestamp = 0;
};
