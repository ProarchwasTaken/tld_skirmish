// hud_life.h
#pragma once
#include <raylib.h>
#include <cstdint>
#include "utils/sequence.h"
#include "combatants/char_player.h"


/* The LifeHud is a hud element that measures the player's health in two
 * forms. By text detailing the exact amount of health the player has 
 * left, and through a segmented life gauge that is much faster to read.
 * Pretty standard stuff really.*/
class LifeHud {
public:
  LifeHud(PlayerCharacter &player, uint8_t &phase);
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

  void drawLifeText();
  void drawLifeGauge();

  void draw();

private:
  PlayerCharacter *player;

  uint8_t *game_phase;

  float life_percentage;
  
  Vector2 hud_position;
  Color hud_color;
  Vector2 txt_position;

  Vector2 gauge_position;
  Rectangle gauge_source; 

  float gauge_width;
  float segment_width;

  Rectangle segment_source;
  Vector2 segment_position;
  bool display_segment = false;

  float blink_interval;
  float blink_timestamp = 0;

  Sequence<int, 2> seq_critical = Sequence<int, 2>({33, 34});
};
