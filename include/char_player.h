// char_player.h
#pragma once
#include <raylib.h>


/* The playable character, the controllable avatar for the user. The most
 * important game object as it wouldn't be a game without one.*/
class PlayerCharacter {
public:
  PlayerCharacter();
  ~PlayerCharacter();

  void draw();

  void hitboxCorrection();
  void texRectCorrection();
private:
  Vector2 position;

  Rectangle hitbox;
  Vector2 hitbox_position;
  Vector2 hitbox_scale;

  Rectangle tex_rect;
  Vector2 tex_position;
  Vector2 tex_scale;
};
